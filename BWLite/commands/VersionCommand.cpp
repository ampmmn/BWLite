#include "pch.h"
#include "framework.h"
#include "VersionCommand.h"
#include "AboutDlg.h"

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

CString VersionCommand::GetErrorString()
{
	return _T("");
}

BOOL VersionCommand::Match(const CString& strQueryStr)
{
	// ���S��v��r
	return strQueryStr == _T("version");
}

