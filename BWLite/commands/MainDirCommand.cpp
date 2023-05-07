#include "pch.h"
#include "framework.h"
#include "commands/MainDirCommand.h"
#include "commands/ShellExecCommand.h"
#include "AppProfile.h"
#include "IconLoader.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MainDirCommand::MainDirCommand()
{
}

MainDirCommand::~MainDirCommand()
{
}

CString MainDirCommand::GetName()
{
	return _T("maindir");
}

CString MainDirCommand::GetDescription()
{
	return _T("�y���C���t�H���_�z");
}

BOOL MainDirCommand::Execute()
{
	TCHAR mainDirPath[65536];
	GetModuleFileName(NULL, mainDirPath, 65536);
	PathRemoveFileSpec(mainDirPath);
	_tcscat_s(mainDirPath, _T("\\"));

	ShellExecCommand cmd;
	cmd.SetPath(mainDirPath);
	return cmd.Execute();
}

BOOL MainDirCommand::Execute(const std::vector<CString>& args)
{
	// �����w�肵�Ă�����͂����Ȃ�
	return Execute();
}

CString MainDirCommand::GetErrorString()
{
	return _T("");
}

HICON MainDirCommand::GetIcon()
{
	return IconLoader::Get()->LoadMainDirIcon();
}

BOOL MainDirCommand::Match(Pattern* pattern)
{
	return pattern->Match(GetName());
}

Command* MainDirCommand::Clone()
{
	return new MainDirCommand();
}

