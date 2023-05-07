#include "pch.h"
#include "framework.h"
#include "VersionCommand.h"
#include "AboutDlg.h"
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
	return _T("�y�o�[�W�������z");
}

BOOL VersionCommand::Execute()
{
	CAboutDlg dlg;
	dlg.DoModal();
	return TRUE;
}

BOOL VersionCommand::Execute(const std::vector<CString>& args)
{
	// �����w�肵�Ă�����͂����Ȃ�
	return Execute();
}

CString VersionCommand::GetErrorString()
{
	return _T("");
}

HICON VersionCommand::GetIcon()
{
	return AfxGetApp()->LoadIcon(IDI_ICON2);
}


BOOL VersionCommand::Match(Pattern* pattern)
{
	return pattern->Match(GetName());
}

