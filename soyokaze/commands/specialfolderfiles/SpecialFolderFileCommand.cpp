#include "pch.h"
#include "framework.h"
#include "SpecialFolderFileCommand.h"
#include "IconLoader.h"
#include "AppPreference.h"
#include "utility/LastErrorString.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace soyokaze {
namespace commands {
namespace specialfolderfiles {


struct SpecialFolderFileCommand::PImpl
{
	ITEM mItem;
};


SpecialFolderFileCommand::SpecialFolderFileCommand(const ITEM& item) : 
	AdhocCommandBase(item.mName, item.mDescription),
	in(std::make_unique<PImpl>())
{
	in->mItem = item;
	if (item.mDescription.IsEmpty()) {
		this->mDescription = item.mFullPath;
	}
}

SpecialFolderFileCommand::~SpecialFolderFileCommand()
{
}


CString SpecialFolderFileCommand::GetTypeDisplayName()
{
	if (in->mItem.mType == TYPE_RECENT) {
		static CString TEXT_TYPE_RECENT((LPCTSTR)IDS_COMMAND_RECENTFILES);
		return TEXT_TYPE_RECENT;
	}
	else {
		static CString TEXT_TYPE_STARTMENU((LPCTSTR)IDS_COMMAND_STARTMENU);
		return TEXT_TYPE_STARTMENU;
	}
}

BOOL SpecialFolderFileCommand::Execute(const Parameter& param)
{
	std::vector<CString> args;
	param.GetParameters(args);

	CString path = in->mItem.mFullPath;
	CString paramStr;

	auto pref = AppPreference::Get();

	bool isOpenPath = pref->IsShowFolderIfCtrlKeyIsPressed() &&
	                  (param.GetNamedParamBool(_T("CtrlKeyPressed")) && PathFileExists(path));
	if (isOpenPath) {
		// $BEPO?$5$l$?%U%!%$%i!<$G3+$/(B
		if (pref->IsUseFiler()) {
			path = pref->GetFilerPath();
			paramStr = pref->GetFilerParam();

			// $B$H$j$"$($:%j%s%/@h$N$_$r%5%]!<%H(B
			paramStr.Replace(_T("$target"), in->mItem.mFullPath);
			//
		}
		else {
			// $BEPO?$5$l$?%U%!%$%i!<$,$J$$>l9g$O%(%/%9%W%m!<%i$G3+$/(B
			if (PathIsDirectory(path) == FALSE) {
				PathRemoveFileSpec(path.GetBuffer(MAX_PATH_NTFS));
				path.ReleaseBuffer();
			}
			paramStr = _T("open");
		}
	}

	SHELLEXECUTEINFO si = {};
	si.cbSize = sizeof(si);
	si.nShow = SW_NORMAL;
	si.fMask = SEE_MASK_NOCLOSEPROCESS;
	si.lpFile = path;
	if (paramStr.IsEmpty() == FALSE) {
		si.lpParameters = paramStr;
	}

	if (ShellExecuteEx(&si) == FALSE) {
		LastErrorString errStr(GetLastError());
		this->mErrMsg = (LPCTSTR)errStr;
		return FALSE;
	}
	CloseHandle(si.hProcess);

	return TRUE;
}

HICON SpecialFolderFileCommand::GetIcon()
{
	return IconLoader::Get()->LoadIconFromPath(in->mItem.mFullPath);
}

soyokaze::core::Command*
SpecialFolderFileCommand::Clone()
{
	return new SpecialFolderFileCommand(in->mItem);
}

} // end of namespace specialfolderfiles
} // end of namespace commands
} // end of namespace soyokaze

