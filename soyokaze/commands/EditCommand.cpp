#include "pch.h"
#include "framework.h"
#include "commands/EditCommand.h"
#include "CommandRepository.h"
#include "IconLoader.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

EditCommand::EditCommand(CommandRepository* cmdMapPtr) : mCmdMapPtr(cmdMapPtr)
{
}

EditCommand::~EditCommand()
{
}

CString EditCommand::GetName()
{
	return _T("edit");
}

CString EditCommand::GetDescription()
{
	return _T("【編集】");
}

BOOL EditCommand::Execute()
{
	std::vector<CString> argEmpty;
	return Execute(argEmpty);
}

BOOL EditCommand::Execute(const std::vector<CString>& args)
{
	if (args.empty()) {
		// キーワードマネージャを実行する
		mCmdMapPtr->ManagerDialog();
		return TRUE;
	}

	CString editName = args[0];
	if (mCmdMapPtr->QueryAsWholeMatch(editName) == nullptr) {
		CString msgStr((LPCTSTR)IDS_ERR_NAMEDOESNOTEXIST);
		msgStr += _T("\n\n");
		msgStr += editName;
		AfxMessageBox(msgStr);
		return TRUE;
	}

	mCmdMapPtr->EditCommandDialog(editName);
	return TRUE;

	return TRUE;
}

CString EditCommand::GetErrorString()
{
	return _T("");
}

HICON EditCommand::GetIcon()
{
	return IconLoader::Get()->LoadNewIcon();
}

BOOL EditCommand::Match(Pattern* pattern)
{
	return pattern->Match(GetName());
}

Command* EditCommand::Clone()
{
	return new EditCommand(mCmdMapPtr);
}

