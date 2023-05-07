#pragma once

class IconLabel;
class CommandRepository;

class CommandEditDialog : public CDialogEx
{
public:
	CommandEditDialog(CommandRepository* cmdMapPtr);
	virtual ~CommandEditDialog();

	void SetOrgName(const CString& name);
	void SetName(const CString& name);
	void SetPath(const CString& path);

	int GetShowType();
	void SetShowType(int type);

	bool UpdateStatus();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual BOOL OnInitDialog();

	CommandRepository* mCmdMapPtr;

	// �ҏW�J�n���̃R�}���h��
	CString mOrgName;

	// ���b�Z�[�W��
	CString mMessage;

public:
	// �R�}���h��
	CString mName;
	// ����
	CString mDescription;
	// �Ǘ��Ҍ����Ŏ��s
	BOOL mIsRunAsAdmin;
	// �\�����@
	int mShowType;
	// �J�����g�f�B���N�g��
	CString mDir;
	// �p�X
	CString mPath;
	// �p�����[�^
	CString mParameter;
	// �p�X(�����Ȃ���)
	CString mPath0;
	// �p�����[�^(�����Ȃ���)
	CString mParameter0;
	// �����Ȃ��ł��g����?
	BOOL mIsUse0;

	IconLabel* mIconLabelPtr;
// ����
protected:
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEditNameChanged();
	afx_msg void OnEditPathChanged();
	afx_msg void OnEditPath0Changed();
	afx_msg void OnButtonBrowseFile1Clicked();
	afx_msg void OnButtonBrowseDir1Clicked();
	afx_msg void OnButtonBrowseFile2Clicked();
	afx_msg void OnButtonBrowseDir2Clicked();
	afx_msg void OnButtonBrowseDir3Clicked();
	afx_msg void OnCheckUse0();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

