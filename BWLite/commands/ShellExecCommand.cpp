#include "pch.h"
#include "framework.h"
#include "ShellExecCommand.h"
#include "AppPreference.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ShellExecCommand::ATTRIBUTE::ATTRIBUTE() :
	mShowType(SW_NORMAL)
{
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


ShellExecCommand::ShellExecCommand()
{
}

ShellExecCommand::~ShellExecCommand()
{
}

CString ShellExecCommand::GetName()
{
	return mName;
}


CString ShellExecCommand::GetDescription()
{
	return mDescription.IsEmpty() ? mName : mDescription;
}

BOOL ShellExecCommand::Execute()
{
	std::vector<CString> argsEmpty;
	return Execute(argsEmpty);
}

BOOL ShellExecCommand::Execute(const std::vector<CString>& args)
{
	// �p�����[�^����/�Ȃ��ŁAmNormalAttr/mNoParamAttr��؂�ւ���
	const ATTRIBUTE& attrPtr = SelectAttribute(args);

	CString path;
	CString param;
	// Ctrl�L�[��������āA���A�p�X�����݂���ꍇ�̓t�@�C���[�ŕ\��
	bool isOpenPath = (GetAsyncKeyState(VK_CONTROL) & 0x8000) &&
	                  PathFileExists(attrPtr.mPath);

	if (isOpenPath || PathIsDirectory(attrPtr.mPath)) {

		// �o�^���ꂽ�t�@�C���[�ŊJ��
		AppPreference pref;
		pref.Load();

		path = pref.GetFilerPath();
		param = pref.GetFilerParam();
		// �Ƃ肠���������N��݂̂��T�|�[�g
		param.Replace(_T("$target"), attrPtr.mPath);
	}
	else {
		path = attrPtr.mPath;
		param = attrPtr.mParam;
	}

	// args�̒l��W�J
	for (int i = 0; i < (int)args.size(); ++i) {
		CString key;
		key.Format(_T("$%d"), i+1);

		path.Replace(key, args[i]);
		param.Replace(key, args[i]);
	}
	// ToDo: ���ϐ��̓W�J


	SHELLEXECUTEINFO si = SHELLEXECUTEINFO();
	si.cbSize = sizeof(si);
	si.nShow = attrPtr.mShowType;
	si.fMask = SEE_MASK_NOCLOSEPROCESS;
	si.lpFile = path;
	if (param.IsEmpty() == FALSE) {
		si.lpParameters = param;
	}
	if (attrPtr.mDir.IsEmpty() == FALSE) {
		si.lpDirectory = attrPtr.mDir;
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

ShellExecCommand& ShellExecCommand::SetName(LPCTSTR name)
{
	mName = name;
	return *this;
}

ShellExecCommand& ShellExecCommand::SetDescription(LPCTSTR description)
{
	mDescription = description;
	return *this;
}


ShellExecCommand& ShellExecCommand::SetAttribute(const ATTRIBUTE& attr)
{
	mNormalAttr = attr;
	return *this;
}

ShellExecCommand& ShellExecCommand::SetAttributeForParam0(const ATTRIBUTE& attr)
{
	mNoParamAttr = attr;
	return *this;
}

ShellExecCommand& ShellExecCommand::SetPath(LPCTSTR path)
{
	mNormalAttr.mPath = path;
	return *this;
}

ShellExecCommand::ATTRIBUTE&
ShellExecCommand::SelectAttribute(
	const std::vector<CString>& args
)
{
	// �p�����[�^�̗L���Ȃǂ�ATTRIBUTE��؂�ւ���

	if (args.size() > 0) {
		// �p�����[�^����

		// mNormalAttr�D��
		if (mNormalAttr.mPath.IsEmpty() == FALSE) {
			return mNormalAttr;
		}

		return mNoParamAttr;
	}
	else {
		// �p�����[�^�Ȃ�

		// mNoParamAttr�D��
		if (mNoParamAttr.mPath.IsEmpty() == FALSE) {
			return mNoParamAttr;
		}

		return mNormalAttr;
	}
}
