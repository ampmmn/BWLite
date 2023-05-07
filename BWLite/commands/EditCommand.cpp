#include "pch.h"
#include "framework.h"
#include "commands/EditCommand.h"
#include "CommandMap.h"
#include "IconLoader.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

EditCommand::EditCommand(CommandMap* cmdMapPtr) : mCmdMapPtr(cmdMapPtr)
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
	return _T("�y�ҏW�z");
}

BOOL EditCommand::Execute()
{
	std::vector<CString> argEmpty;
	return Execute(argEmpty);
}

BOOL EditCommand::Execute(const std::vector<CString>& args)
{
	if (args.empty()) {
		// �L�[���[�h�}�l�[�W�������s����
		mCmdMapPtr->ManagerDialog();
		return TRUE;
	}

	mCmdMapPtr->EditCommandDialog(args[0]);
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

