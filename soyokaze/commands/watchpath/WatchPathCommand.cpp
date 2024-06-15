#include "pch.h"
#include "framework.h"
#include "WatchPathCommand.h"
#include "commands/watchpath/WatchPathCommandEditDialog.h"
#include "commands/watchpath/PathWatcher.h"
#include "commands/shellexecute/ShellExecCommand.h"
#include "commands/common/ExpandFunctions.h"
#include "commands/core/CommandRepository.h"
#include "setting/AppPreference.h"
#include "commands/core/CommandFile.h"
#include "icon/IconLoader.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace launcherapp {
namespace commands {
namespace watchpath {

using namespace launcherapp::commands::common;

using CommandRepository = launcherapp::core::CommandRepository;
using ShellExecCommand = launcherapp::commands::shellexecute::ShellExecCommand;

struct WatchPathCommand::PImpl
{
	CString mName;
	CString mDescription;
	CString mPath;
	CString mMessage;
	bool mIsDisabled = false;
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

CString WatchPathCommand::GetType() { return _T("WatchPath"); }

WatchPathCommand::WatchPathCommand() : in(std::make_unique<PImpl>())
{
}

WatchPathCommand::~WatchPathCommand()
{
}

CString WatchPathCommand::GetName()
{
	return in->mName;
}


CString WatchPathCommand::GetDescription()
{
	return in->mDescription;
}

CString WatchPathCommand::GetGuideString()
{
	return _T("Enter:更新検知対象パスを開く");
}

CString WatchPathCommand::GetTypeDisplayName()
{
	return _T("フォルダ更新検知");
}

BOOL WatchPathCommand::Execute(const Parameter& param)
{
	if (in->mIsDisabled) {
		return TRUE;
	}

	auto path = in->mPath;
	path += _T("\\");

	ShellExecCommand cmd;
	cmd.SetPath(path);

	Parameter paramEmpty;
	return cmd.Execute(paramEmpty);
}

CString WatchPathCommand::GetErrorString()
{
	return _T("");
}

HICON WatchPathCommand::GetIcon()
{
	CString path = in->mPath;
	ExpandMacros(path);

	return IconLoader::Get()->LoadIconFromPath(path);
}

int WatchPathCommand::Match(Pattern* pattern)
{
	return pattern->Match(GetName());
}

int WatchPathCommand::EditDialog(const Parameter* param)
{
	auto oldPath = in->mPath;

	CommandEditDialog dlg;
	dlg.SetOrgName(in->mName);

	dlg.mName = in->mName;
	dlg.mDescription = in->mDescription;
	dlg.mPath = in->mPath;
	dlg.mNotifyMessage = in->mMessage;
	dlg.mIsDisabled = in->mIsDisabled ? TRUE : FALSE;

	if (dlg.DoModal() != IDOK) {
		return 1;
	}

	// 名前が変わっている可能性があるため、いったん削除して再登録する #addref
	auto cmdRepo = CommandRepository::GetInstance();

	AddRef();  // UnregisterCommandで削除されるのを防ぐため+1しておく
	cmdRepo->UnregisterCommand(this);

	CString orgName = in->mName;

	in->mName = dlg.mName;
	in->mDescription = dlg.mDescription;
	in->mPath = dlg.mPath;
	in->mMessage = dlg.mNotifyMessage;
	in->mIsDisabled = (dlg.mIsDisabled != FALSE);

	// RegisterCommandはrefCountを+1しないので、#addrefで上げたカウントをさげる必要はない
	cmdRepo->RegisterCommand(this);

	// 登録しなおす
	auto watcher = PathWatcher::Get();
	watcher->UnregisterPath(orgName);

	if (in->mIsDisabled == false) {
		PathWatcher::ITEM item;
		item.mPath = dlg.mPath;
		item.mMessage = dlg.mNotifyMessage;
		watcher->RegisterPath(in->mName, item);
	}

	return 0;
}

/**
 *  @brief 優先順位の重みづけを使用するか?
 *  @true true:優先順位の重みづけを使用する false:使用しない
 */
bool WatchPathCommand::IsPriorityRankEnabled()
{
	return false;
}

launcherapp::core::Command*
WatchPathCommand::Clone()
{
	auto clonedObj = std::make_unique<WatchPathCommand>();

	clonedObj->in->mName = in->mName;
	clonedObj->in->mDescription = in->mDescription;
	clonedObj->in->mPath = in->mPath;
	clonedObj->in->mIsDisabled = in->mIsDisabled;

	return clonedObj.release();
}

bool WatchPathCommand::Save(CommandEntryIF* entry)
{
	ASSERT(entry);

	entry->Set(_T("Type"), GetType());

	entry->Set(_T("description"), GetDescription());
	entry->Set(_T("path"), in->mPath);
	entry->Set(_T("message"), in->mMessage);
	entry->Set(_T("isDisabled"), in->mIsDisabled);

	return true;
}

bool WatchPathCommand::Load(CommandEntryIF* entry)
{
	ASSERT(entry);

	CString typeStr = entry->Get(_T("Type"), _T(""));
	if (typeStr.IsEmpty() == FALSE && typeStr != GetType()) {
		return false;
	}

	in->mName = entry->GetName();
	in->mDescription = entry->Get(_T("description"), _T(""));
	in->mPath = entry->Get(_T("path"), _T(""));
	in->mMessage = entry->Get(_T("message"), _T(""));
	in->mIsDisabled = entry->Get(_T("isDisabled"), false);

	// 監視対象に登録
	PathWatcher::ITEM item;
	item.mPath = in->mPath;
	item.mMessage = in->mMessage;
	PathWatcher::Get()->RegisterPath(in->mName, item);

	return true;
}

bool WatchPathCommand::NewDialog(const Parameter* param)
{
	// 新規作成ダイアログを表示
	CString value;

	CommandEditDialog dlg;
	if (param && param->GetNamedParam(_T("COMMAND"), &value)) {
		dlg.SetName(value);
	}
	if (param && param->GetNamedParam(_T("DESCRIPTION"), &value)) {
		dlg.SetDescription(value);
	}
	if (dlg.DoModal() != IDOK) {
		return false;
	}

	// ダイアログで入力された内容に基づき、コマンドを新規作成する
	auto newCmd = std::make_unique<WatchPathCommand>();
	newCmd->in->mName = dlg.mName;
	newCmd->in->mDescription = dlg.mDescription;
	newCmd->in->mPath = dlg.mPath;
	newCmd->in->mMessage = dlg.mNotifyMessage;
	newCmd->in->mIsDisabled = dlg.mIsDisabled != FALSE;

	CommandRepository::GetInstance()->RegisterCommand(newCmd.release());

	// 監視対象に登録
	if (newCmd->in->mIsDisabled == false) {
		PathWatcher::ITEM item;
		item.mPath = dlg.mPath;
		item.mMessage = dlg.mNotifyMessage;
		PathWatcher::Get()->RegisterPath(dlg.mName, item);
	}

	return true;

}

}
}
}

