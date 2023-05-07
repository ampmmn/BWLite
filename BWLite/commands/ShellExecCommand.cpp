#include "pch.h"
#include "framework.h"
#include "ShellExecCommand.h"
#include "AppPreference.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ShellExecCommand::ShellExecCommand() :
	m_nShowType(SW_NORMAL), m_bEnable(true)
{
}

ShellExecCommand::~ShellExecCommand()
{
}

CString ShellExecCommand::GetName()
{
	return m_strName;
}


CString ShellExecCommand::GetDescription()
{
	return m_strDescription.IsEmpty() ? m_strName : m_strDescription;
}

BOOL ShellExecCommand::Execute()
{
	// Ctrl�L�[��������āA���A�p�X�����݂���ꍇ�̓t�@�C���[�ŕ\��
	bool isOpenPath = (GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
	                  PathFileExists(m_strPath);

	CString path;
	CString param;
	if (isOpenPath || PathIsDirectory(m_strPath)) {

		// �o�^���ꂽ�t�@�C���[�ŊJ��
		AppPreference pref;
		pref.Load();

		path = pref.GetFilerPath();
		param = pref.GetFilerParam();
		// �Ƃ肠���������N��݂̂��T�|�[�g
		param.Replace(_T("$1"), m_strPath);
	}
	else {
		path = m_strPath;
		param = m_strParam;
	}

	SHELLEXECUTEINFO si = SHELLEXECUTEINFO();
	si.cbSize = sizeof(si);
	si.nShow = m_nShowType;
	si.fMask = SEE_MASK_NOCLOSEPROCESS;
	si.lpFile = path;
	if (param.IsEmpty() == FALSE) {
		si.lpParameters = param;
	}
	if (m_strDir.IsEmpty() == FALSE) {
		si.lpDirectory = m_strDir;
	}
	BOOL bRun = ShellExecuteEx(&si);
	if (bRun == FALSE) {
		return FALSE;
	}

	CloseHandle(si.hProcess);

	return TRUE;
}

CString ShellExecCommand::GetErrorString()
{
	return _T("");
}

BOOL ShellExecCommand::Match(const CString& strQueryStr)
{
	// ���������ꂽ�R�}���h�̓}�b�`�Ώۂ��珜�O����
	if (m_bEnable == false) {
		return FALSE;
	}

	// ToDo: �啶���������̋�ʂ�؂�ւ�����悤��
	// ToDo: �v�f�̔�r�̂��т�strTmp��肽���Ȃ�

	// �O����v����
	//return m_strName.Find(strQueryStr) == 0;

	// �啶���������𖳎������O����v����
	CString strTmp = strQueryStr;
	strTmp.MakeLower();
	return m_strNameForIgnoreCase.Find(strTmp) == 0;
}


ShellExecCommand& ShellExecCommand::SetName(LPCTSTR name)
{
	m_strName = name;
	m_strNameForIgnoreCase = name;
	m_strNameForIgnoreCase.MakeLower();
	return *this;
}

ShellExecCommand& ShellExecCommand::SetPath(LPCTSTR path)
{
	m_strPath = path;
	return *this;
}

ShellExecCommand& ShellExecCommand::SetParam(LPCTSTR param)
{
	m_strParam = param;
	return *this;
}

ShellExecCommand& ShellExecCommand::SetDirectory(LPCTSTR dir)
{
	m_strDir = dir;
	return *this;
}

ShellExecCommand& ShellExecCommand::SetDescription(LPCTSTR description)
{
	m_strDescription = description;
	return *this;
}

ShellExecCommand& ShellExecCommand::SetShowType(int showType)
{
	m_nShowType = showType;
	return *this;
}

ShellExecCommand& ShellExecCommand::SetEnable(bool isEnable)
{
	m_bEnable = isEnable;
	return *this;
}

