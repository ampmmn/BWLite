#include "pch.h"
#include "framework.h"
#include "URLDirectoryIndexCommand.h"
#include "commands/url_directoryindex/URLDirectoryIndexCommandParam.h"
#include "commands/url_directoryindex/URLDirectoryIndexCommandEditDialog.h"
#include "commands/url_directoryindex/WinHttp.h"
#include "commands/core/CommandRepository.h"
#include "hotkey/CommandHotKeyManager.h"
#include "hotkey/CommandHotKeyMappings.h"
#include "setting/AppPreference.h"
#include "commands/core/CommandFile.h"
#include "icon/IconLoader.h"
#include "SharedHwnd.h"
#include "resource.h"
#include "commands/activate_window/AutoWrap.h"
#include "utility/CharConverter.h"
#include "utility/AES.h"

#include <deque>
#include <map>
#include <tidy.h>
#include <tidybuffio.h>
#include "spdlog/stopwatch.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace launcherapp::commands::common;

using CommandRepository = launcherapp::core::CommandRepository;

namespace launcherapp {
namespace commands {
namespace url_directoryindex {

using LinkItems = std::vector<std::pair<CString, CString> >;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


constexpr LPCTSTR TYPENAME = _T("URLDirectoryIndexCommand");

struct URLDirectoryIndexCommand::PImpl
{
	PImpl()
	{
	}
	~PImpl()
	{
	}
	bool LoadContent(const CString& url, std::vector<BYTE>& content, bool& isHTML);
	bool ExtractCandidates(const std::vector<BYTE>& content, LinkItems& items);

	void Query(Pattern* pattern, DirectoryIndexQueryResult& results);

	bool IsLoaded()
	{
		return mIsLoaded;
	}

	size_t GetCandidatesCount()
	{
		return mLinkItems.size();
	}

	void ClearCache()
	{
		mLinkCache.clear();
	}
	void Reset()
	{
		mLinkItems.clear();
		mIsEmpty = false;
		mIsLoaded = false;
		mIsLoadOK = false;
	}

	std::vector<uint8_t>& Encode(const CString& str, std::vector<uint8_t>& buf);
	CString Decode(const std::vector<uint8_t>& buf);

	CommandParam mParam;
	CommandHotKeyAttribute mHotKeyAttr;
	CString mErrMsg;

	LinkItems mLinkItems;
	std::map<CString, LinkItems> mLinkCache;

	CString mSubPath;

	bool mIsEmpty = false;
	bool mIsLoaded = false;
	bool mIsLoadOK = false;
};

// 候補一覧の生成を開始する
bool URLDirectoryIndexCommand::PImpl::LoadContent(const CString& url, std::vector<BYTE>& content, bool& isHTML)
{
	WinHttp http;
	if (mParam.mServerUser.IsEmpty() == FALSE) {
		http.SetServerCredential(mParam.mServerUser, mParam.mServerPassword);
	}
	http.SetProxyType(mParam.mProxyType);
	http.SetProxyCredential(mParam.mProxyHost, mParam.mProxyUser, mParam.mProxyPassword);

	return http.LoadContent(url, content, isHTML);
}

static tregex& GetURLRegex()
{
	static tregex regURL(_T("https?://.+"));
	return regURL;
}


// HTMLからリンク一覧を生成する
bool URLDirectoryIndexCommand::PImpl::ExtractCandidates(
	const std::vector<BYTE>& content,
	LinkItems& items
)
{
	launcherapp::utility::CharConverter conv;

 	TidyDoc doc = tidyCreate();
	tidyOptSetBool(doc, TidyForceOutput, yes);

	int err = tidyParseString(doc, (LPCSTR)content.data());
	if (err < 0) {
		mIsLoadOK = false;
		mIsLoaded = true;
		return false;
	}

	err = tidyCleanAndRepair(doc);
	err = tidyRunDiagnostics(doc);

	auto rootNode = tidyGetRoot(doc);

	if (rootNode == nullptr) {
		spdlog::debug(_T("tidyGetRoot returned nullptr."));
	}

	std::deque<TidyNode> stk;
	stk.push_back(rootNode);

	LinkItems links;

	while(stk.empty() == false) {
		auto curNode = stk.front();
		stk.pop_front();

		auto curName = tidyNodeGetName(curNode);


		for (TidyNode childNode = tidyGetChild(curNode); childNode != nullptr;
		     childNode = tidyGetNext(childNode)) {

			auto name = tidyNodeGetName(childNode);
			if(name) {				

				CString href;
				bool isA = stricmp(name, "a") == 0;

				for(auto attr = tidyAttrFirst(childNode); attr; attr = tidyAttrNext(attr) ) {
					auto attrName = tidyAttrName(attr);
					auto attrValue = tidyAttrValue(attr);
					if (isA && stricmp(attrName, "href") == 0) {
						conv.Convert(attrValue, href);
					}
				}
				stk.push_back(childNode);

				// apacheのDirectory IndexのName/Last modified/Size/Descriptionのソートは出さないことにする
				bool isSortLink = (href.Find(_T("?C=")) == 0);

				// 遷移先が絶対URLであるものを除外する
				// (他ドメインに遷移するのを防ぐため)
				static tregex regURL = GetURLRegex();
				bool isAbsURL = std::regex_search((LPCTSTR)href, regURL);

				if (isA== false || isSortLink || isAbsURL) {
					continue ;
				}

				auto textNode = tidyGetChild(childNode);

				if (textNode == nullptr) {
					continue;
				}

				TidyBuffer buf;
				tidyBufInit(&buf);
				tidyNodeGetText(doc, textNode, &buf);

				CString displayName;
				conv.Convert((const char*)buf.bp, displayName);
				if (isA) {
					displayName.Trim();

					// 表示テキストが"Parent Directory"だったら、".."に置き換える
					if (displayName.CompareNoCase(_T("Parent Directory")) == 0) {
						displayName = _T("..");
					}

					links.push_back(std::pair<CString, CString>(href, displayName));
				}
				tidyBufFree(&buf);
			}
			stk.push_back(childNode);
		}
	}
 
  tidyRelease(doc);

	items.swap(links);

	return true;
}

void URLDirectoryIndexCommand::PImpl::Query(Pattern* pattern, DirectoryIndexQueryResult& results)
{
	DirectoryIndexQueryResult tmpList;

	std::vector<CString> words;
	pattern->GetRawWords(words);
	if (words.size() == 1) {
		// 入力キーワードが空文字の場合は全てを候補に追加する
		for(auto& item : mLinkItems) {
			QueryResult result;
			result.mLinkPath = item.first;
			result.mLinkText = item.second;
			result.mURL = mParam.CombineURL(item.first);
			result.mMatchLevel = Pattern::FrontMatch;
			tmpList.push_back(result);
		}
		tmpList.swap(results);
		return;
	}


	for(auto& item : mLinkItems) {
		int level = pattern->Match(item.second, 1);
		if (level == Pattern::Mismatch) {
			continue;
		}
		if (level == Pattern::PartialMatch) {
			// 先行キーワードは一致しているので前方一致扱いとする
			level = Pattern::FrontMatch;
		}

		QueryResult result;
		result.mLinkPath = item.first;
		result.mLinkText = item.second;
		result.mURL = mParam.CombineURL(item.first);
		result.mMatchLevel = level;

		tmpList.push_back(result);
	}

	tmpList.swap(results);
}


std::vector<uint8_t>& URLDirectoryIndexCommand::PImpl::Encode(const CString& str, std::vector<uint8_t>& buf)
{
	::utility::aes::AES aes;
	aes.SetPassphrase("aiueo");  // てきとうa

	int len = str.GetLength() + 1;
	std::vector<uint8_t> plainData(len * sizeof(TCHAR));
	memcpy(plainData.data(), (LPCTSTR)str, plainData.size());

	aes.Encrypt(plainData, buf);
	return buf;
}

CString URLDirectoryIndexCommand::PImpl::Decode(const std::vector<uint8_t>& src)
{
	::utility::aes::AES aes;
	aes.SetPassphrase("aiueo");  // てきとう

	std::vector<uint8_t> plainData;
	if (aes.Decrypt(src, plainData) == false) {
		return _T("");
	}

	int len = (int)plainData.size();
	CString str;
	memcpy(str.GetBuffer(len), plainData.data(), len);
	str.ReleaseBuffer();

	return str;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

CString URLDirectoryIndexCommand::GetType() { return _T("URLDirectoryIndex"); }

URLDirectoryIndexCommand::URLDirectoryIndexCommand() : in(std::make_unique<PImpl>())
{
}

URLDirectoryIndexCommand::~URLDirectoryIndexCommand()
{
}

void URLDirectoryIndexCommand::SetSubPath(const CString& subPath)
{
	// https?から始まる場合はURLとして扱う
	// '/'で始まる場合は絶対パスとして扱う
	static tregex regURL = GetURLRegex();
	if (subPath.Left(1) == _T('/') || std::regex_search((LPCTSTR)subPath, regURL)) {
		in->mSubPath = subPath;
	}
	else {
		// 相対パスの場合はルートから
		if (in->mSubPath.IsEmpty() == FALSE && in->mSubPath.Right(1) != _T('/')) {
			in->mSubPath += _T('/');
		}
		in->mSubPath += subPath;
	}

	in->Reset();
}

CString URLDirectoryIndexCommand::GetSubPath()
{
	return in->mSubPath;
}

void URLDirectoryIndexCommand::LoadCanidates(bool& isHTML)
{
	CString url = in->mParam.CombineURL(in->mSubPath);

	// キャッシュがあればそれを使う
	auto itCache = in->mLinkCache.find(url);
	if (itCache != in->mLinkCache.end()) {
		in->mLinkItems = itCache->second;
		isHTML = true;
	}
	else {
		// なければダウンロード
		std::vector<BYTE> content;
		if (in->LoadContent(url, content, isHTML) == false) {
			return;
		}
		// 取得したテキストを解析
		spdlog::debug("extract candidates");
		LinkItems items;
		if (in->ExtractCandidates(content, items) == false) {
			return;
		}

		// リンクがなかったらその旨を示すダミー項目を追加する
		if (items.empty()) {
			items.push_back(std::pair<CString, CString>(_T("."), _T("(No links found)")));
		}

		in->mLinkCache[url] = items;
		in->mLinkItems.swap(items);
	}

	in->mIsLoaded = true;
	in->mIsLoadOK = true;
}

void URLDirectoryIndexCommand::Query(Pattern* pattern, DirectoryIndexQueryResult& results)
{
	// コマンド名が一致しなければ候補を表示しない
	if (GetName().CompareNoCase(pattern->GetFirstWord()) != 0) {
		in->Reset();
		in->mSubPath.Empty();
		spdlog::info(_T("clear state"));
		return;
	}
	in->Query(pattern, results);
}

CString URLDirectoryIndexCommand::GetName()
{
	return in->mParam.mName;
}


CString URLDirectoryIndexCommand::GetDescription()
{
	return in->mParam.mDescription;
}

CString URLDirectoryIndexCommand::GetGuideString()
{
	return _T("Enter:候補を表示する");
}

/**
 * 種別を表す文字列を取得する
 * @return 文字列
 */
CString URLDirectoryIndexCommand::GetTypeName()
{
	return TYPENAME;
}

CString URLDirectoryIndexCommand::GetTypeDisplayName()
{
	static CString TEXT_TYPE(_T("DirectiryIndex"));
	return TEXT_TYPE;
}

BOOL URLDirectoryIndexCommand::Execute(const Parameter& param)
{
	UNREFERENCED_PARAMETER(param);

	if (in->mIsEmpty) {
		// 候補が存在しないとわかっている場合は何もしない
		return TRUE;
	}

	SharedHwnd sharedWnd;
	SendMessage(sharedWnd.GetHwnd(), WM_APP + 2, 1, 0);

	auto cmdline = GetName();
	cmdline += _T(" ");
	SendMessage(sharedWnd.GetHwnd(), WM_APP+11, 0, (LPARAM)(LPCTSTR)cmdline);
	return TRUE;
}

CString URLDirectoryIndexCommand::GetErrorString()
{
	return in->mErrMsg;
}

URLDirectoryIndexCommand& URLDirectoryIndexCommand::SetParam(const CommandParam& param)
{
	in->mParam = param;
	return *this;
}

URLDirectoryIndexCommand& URLDirectoryIndexCommand::GetParam(CommandParam& param)
{
	param = in->mParam;
	return *this;
}


HICON URLDirectoryIndexCommand::GetIcon()
{
	return IconLoader::Get()->GetShell32Icon(-63011);
}

int URLDirectoryIndexCommand::Match(Pattern* pattern)
{
	in->mIsEmpty = false;

	if (pattern->shouldWholeMatch() && pattern->Match(GetName()) == Pattern::WholeMatch) {
		// 内部のコマンド名マッチング用の判定
		return Pattern::WholeMatch;
	}
	else if (pattern->shouldWholeMatch() == false) {

		// 入力欄からの入力で、前方一致するときは候補に出す
		int level = pattern->Match(GetName());
		if (level == Pattern::FrontMatch) {
			return Pattern::FrontMatch;
		}
		if (level == Pattern::WholeMatch && pattern->GetWordCount() == 1) {

			if (in->IsLoaded()) {

				// 候補列挙済の場合、候補が存在する場合は、候補をFilterAdhocCommandとして表示するため、
				// このコマンド自身は表示しない
				if (in->GetCandidatesCount() > 0) {
					// 候補一覧生成済の場合は表示しない
					return Pattern::Mismatch;
				}

				// 候補件数が0の場合、その旨をURLDirectoryIndexCommand自身が表示する
				in->mIsEmpty = true;

				return Pattern::WholeMatch;
			}

			// このタイミングで候補一覧の生成を行う
			bool isHTML = false;
			LoadCanidates(isHTML);

			return Pattern::WholeMatch;
		}
	}

	// 通常はこちら
	return Pattern::Mismatch;
}

int URLDirectoryIndexCommand::EditDialog(HWND parent)
{
	URLDirectoryIndexCommandEditDialog dlg(CWnd::FromHandle(parent));
	dlg.SetOrgName(GetName());

	dlg.SetParam(in->mParam);
	dlg.mHotKeyAttr = in->mHotKeyAttr;

	if (dlg.DoModal() != IDOK) {
		spdlog::info(_T("Dialog cancelled."));
		return 1;
	}

	// 変更後の設定値で上書き
	dlg.GetParam(in->mParam);
	in->mHotKeyAttr = dlg.mHotKeyAttr;

	// 名前の変更を登録しなおす
	auto cmdRepo = launcherapp::core::CommandRepository::GetInstance();
	cmdRepo->ReregisterCommand(this);

	// 設定変更を反映するため、候補のキャッシュを消す
	in->ClearCache();
	in->Reset();

	return 0;
}

bool URLDirectoryIndexCommand::GetHotKeyAttribute(CommandHotKeyAttribute& attr)
{
	attr = in->mHotKeyAttr;
	return true;
}

/**
 *  @brief 優先順位の重みづけを使用するか?
 *  @true true:優先順位の重みづけを使用する false:使用しない
 */
bool URLDirectoryIndexCommand::IsPriorityRankEnabled()
{
	return true;
}

launcherapp::core::Command*
URLDirectoryIndexCommand::Clone()
{
	auto clonedObj = std::make_unique<URLDirectoryIndexCommand>();
	clonedObj->in->mParam = in->mParam;
	return clonedObj.release();
}

bool URLDirectoryIndexCommand::Save(CommandEntryIF* entry)
{
	ASSERT(entry);

	entry->Set(_T("Type"), GetType());

	entry->Set(_T("description"), GetDescription());

	entry->Set(_T("url"), in->mParam.mURL);

	entry->Set(_T("serveruser"), in->mParam.mServerUser);
	std::vector<uint8_t> buf;
	entry->Set(_T("serverpassword"), in->Encode(in->mParam.mServerPassword, buf));

	entry->Set(_T("proxytype"), in->mParam.mProxyType);
	entry->Set(_T("proxyhost"), in->mParam.mProxyHost);
	entry->Set(_T("proxyuser"), in->mParam.mProxyUser);
	entry->Set(_T("proxypassword"), in->Encode(in->mParam.mProxyPassword, buf));


	return true;
}

bool URLDirectoryIndexCommand::Load(CommandEntryIF* entry)
{
	ASSERT(entry);

	CString typeStr = entry->Get(_T("Type"), _T(""));
	if (typeStr.IsEmpty() == FALSE && typeStr != URLDirectoryIndexCommand::GetType()) {
		return false;
	}

	in->mParam.mName = entry->GetName();
	in->mParam.mDescription = entry->Get(_T("description"), _T(""));
	in->mParam.mURL = entry->Get(_T("url"), _T(""));

	in->mParam.mServerUser = entry->Get(_T("serveruser"), _T(""));

	std::vector<uint8_t> buf;
	entry->Get(_T("serverpassword"), buf);
	in->mParam.mServerPassword = in->Decode(buf);

	in->mParam.mProxyType = entry->Get(_T("proxytype"), 0);
	in->mParam.mProxyHost = entry->Get(_T("proxyhost"), _T(""));
	in->mParam.mProxyUser = entry->Get(_T("proxyuser"), _T(""));

	entry->Get(_T("proxypassword"), buf);
	in->mParam.mProxyPassword = in->Decode(buf);

	auto hotKeyManager = launcherapp::core::CommandHotKeyManager::GetInstance();
	hotKeyManager->GetKeyBinding(GetName(), &in->mHotKeyAttr);

	return true;
}

bool URLDirectoryIndexCommand::NewDialog(const Parameter* param, URLDirectoryIndexCommand** newCmd)
{
	// 新規作成ダイアログを表示
	CString value;

	CommandParam tmpParam;

	if (param && param->GetNamedParam(_T("COMMAND"), &value)) {
		tmpParam.mName = value;
	}
	if (param && param->GetNamedParam(_T("PATH"), &value)) {
		tmpParam.mURL = value;
	}
	if (param && param->GetNamedParam(_T("DESCRIPTION"), &value)) {
		tmpParam.mDescription = value;
	}

	URLDirectoryIndexCommandEditDialog dlg;
	dlg.SetParam(tmpParam);

	if (dlg.DoModal() != IDOK) {
		return false;
	}

	// ダイアログで入力された内容に基づき、コマンドを新規作成する
	auto cmd = std::make_unique<URLDirectoryIndexCommand>();

	if (newCmd) {
		*newCmd = cmd.get();
	}

	dlg.GetParam(tmpParam);
	cmd->SetParam(tmpParam);

	constexpr bool isReloadHotKey = true;
	CommandRepository::GetInstance()->RegisterCommand(cmd.release(), isReloadHotKey);

	return true;
}

bool URLDirectoryIndexCommand::CastFrom(launcherapp::core::Command* cmd, URLDirectoryIndexCommand** newCmd)
{
	if (cmd->GetTypeName() != TYPENAME) {
		return false;
	}
	*newCmd = dynamic_cast<URLDirectoryIndexCommand*>(cmd);
	cmd->AddRef();
	return true;
}

} // end of namespace url_directoryindex
} // end of namespace commands
} // end of namespace launcherapp
