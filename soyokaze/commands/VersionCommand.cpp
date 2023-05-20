#include "pch.h"
#include "framework.h"
#include "VersionCommand.h"
#include "gui/AboutDlg.h"
#include "IconLoader.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

VersionCommand::VersionCommand()
{
}

VersionCommand::~VersionCommand()
{
}

CString VersionCommand::GetName()
{
	return _T("version");
}

CString VersionCommand::GetDescription()
{
	return _T("【バージョン情報】");
}

BOOL VersionCommand::Execute()
{
	CAboutDlg dlg;
	dlg.DoModal();
	return TRUE;
}

BOOL VersionCommand::Execute(const std::vector<CString>& args)
{
	// 引数指定しても動作はかわらない
	return Execute();
}

CString VersionCommand::GetErrorString()
{
	return _T("");
}

HICON VersionCommand::GetIcon()
{
	return IconLoader::Get()->LoadVersionIcon();
}


int VersionCommand::Match(Pattern* pattern)
{
	return pattern->Match(GetName());
}

Command* VersionCommand::Clone()
{
	return new VersionCommand();
}

