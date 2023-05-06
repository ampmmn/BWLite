#include "pch.h"
#include "framework.h"
#include "CommandEditDialog.h"
#include "FolderDialog.h"
#include "CommandRepository.h"
#include "IconLabel.h"
#include "IconLoader.h"
#include "resource.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CommandEditDialog::CommandEditDialog(CommandRepository* cmdMapPtr) : 
	CDialogEx(IDD_NEWCOMMAND),
	mCmdMapPtr(cmdMapPtr),
	mIconLabelPtr(new IconLabel)
{
}

CommandEditDialog::~CommandEditDialog()
{
	delete mIconLabelPtr;
}

void CommandEditDialog::SetOrgName(const CString& name)
{
	mOrgName = name;
}

void CommandEditDialog::SetName(const CString& name)
{
	mName = name;
}

int CommandEditDialog::GetShowType()
{
	if (mShowType == 1) {
		return SW_MAXIMIZE;
	}
	else if (mShowType == 2) {
		return SW_SHOWMINIMIZED;
	}
	else {
		return SW_NORMAL;
	}
}

void CommandEditDialog::SetShowType(int type)
{
	if (type == SW_SHOWMINIMIZED) {
		mShowType = 2;
	}
	else if (type == SW_MAXIMIZE) {
		mShowType = 1;
	}
	else {
		mShowType = 0;
	}
}

void CommandEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_STATUSMSG, mMessage);
	DDX_Text(pDX, IDC_EDIT_NAME, mName);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, mDescription);
	DDX_Check(pDX, IDC_CHECK_RUNASADMIN, mIsRunAsAdmin);
	DDX_CBIndex(pDX, IDC_COMBO_SHOWTYPE, mShowType);
	DDX_Text(pDX, IDC_EDIT_PATH, mPath);
	DDX_Text(pDX, IDC_EDIT_PARAM, mParameter);
	DDX_Text(pDX, IDC_EDIT_PATH0, mPath0);
	DDX_Check(pDX, IDC_CHECK_USE0, mIsUse0);
	DDX_Text(pDX, IDC_EDIT_PARAM0, mParameter0);
	DDX_Text(pDX, IDC_EDIT_DIR, mDir);
}

BEGIN_MESSAGE_MAP(CommandEditDialog, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnEditNameChanged)
	ON_EN_CHANGE(IDC_EDIT_PATH, OnEditPathChanged)
	ON_EN_CHANGE(IDC_EDIT_PATH0, OnEditPath0Changed)
	ON_COMMAND(IDC_BUTTON_BROWSEFILE1, OnButtonBrowseFile1Clicked)
	ON_COMMAND(IDC_BUTTON_BROWSEDIR1, OnButtonBrowseDir1Clicked)
	ON_COMMAND(IDC_BUTTON_BROWSEFILE2, OnButtonBrowseFile2Clicked)
	ON_COMMAND(IDC_BUTTON_BROWSEDIR2, OnButtonBrowseDir2Clicked)
	ON_COMMAND(IDC_BUTTON_BROWSEDIR3, OnButtonBrowseDir3Clicked)
	ON_COMMAND(IDC_CHECK_USE0, OnCheckUse0)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


BOOL CommandEditDialog::OnInitDialog()
{
	__super::OnInitDialog();

	SetIcon(IconLoader::Get()->LoadDefaultIcon(), FALSE);

	mIconLabelPtr->SubclassDlgItem(IDC_STATIC_ICON, this);

	CString caption;
  GetWindowText(caption);

	CString suffix;
	suffix.Format(_T("�y%s�z"), mOrgName.IsEmpty() ? _T("�V�K�쐬") : (LPCTSTR)mOrgName);

	caption += suffix;
	SetWindowText(caption);

	// File&Folder Select Button
	GetDlgItem(IDC_BUTTON_BROWSEFILE1)->SetWindowTextW(L"\U0001F4C4");
	GetDlgItem(IDC_BUTTON_BROWSEFILE2)->SetWindowTextW(L"\U0001F4C4");
	GetDlgItem(IDC_BUTTON_BROWSEDIR1)->SetWindowTextW(L"\U0001F4C2");
	GetDlgItem(IDC_BUTTON_BROWSEDIR2)->SetWindowTextW(L"\U0001F4C2");
	GetDlgItem(IDC_BUTTON_BROWSEDIR3)->SetWindowTextW(L"\U0001F4C2");

	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

bool CommandEditDialog::UpdateStatus()
{
	GetDlgItem(IDC_STATIC_PATH0)->EnableWindow(mIsUse0);
	GetDlgItem(IDC_STATIC_PARAM00)->EnableWindow(mIsUse0);
	GetDlgItem(IDC_EDIT_PATH0)->EnableWindow(mIsUse0);
	GetDlgItem(IDC_EDIT_PARAM0)->EnableWindow(mIsUse0);
	GetDlgItem(IDC_BUTTON_BROWSEFILE2)->EnableWindow(mIsUse0);
	GetDlgItem(IDC_BUTTON_BROWSEDIR2)->EnableWindow(mIsUse0);

	mIconLabelPtr->DrawIcon(IconLoader::Get()->LoadIconFromPath(mPath));

	if (mName.IsEmpty()) {
		mMessage.LoadString(IDS_ERR_NAMEISEMPTY);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		return false;
	}
	// �d���`�F�b�N
	if (mName.CompareNoCase(mOrgName) != 0 && 
	    mCmdMapPtr->QueryAsWholeMatch(mName) != nullptr) {
		mMessage.LoadString(IDS_ERR_NAMEALREADYEXISTS);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		return false;
	}

	// �g���Ȃ������`�F�b�N
	if (mCmdMapPtr->IsValidAsName(mName) == false) {
		mMessage.LoadString(IDS_ERR_ILLEGALCHARCONTAINS);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		return false;
	}

	//
	if (mPath.IsEmpty()) {
		mMessage.LoadString(IDS_ERR_PATHISEMPTY);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		return false;
	}

	if (mIsUse0 && mPath0.IsEmpty()) {
		mMessage.LoadString(IDS_ERR_PATH0ISEMPTY);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		return false;
	}

	mMessage.Empty();
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	return true;
}

void CommandEditDialog::OnEditNameChanged()
{
	UpdateData();
	UpdateStatus();
	UpdateData(FALSE);
}

void CommandEditDialog::OnEditPathChanged()
{
	UpdateData();
	UpdateStatus();
	UpdateData(FALSE);
}

void CommandEditDialog::OnEditPath0Changed()
{
	UpdateData();
	UpdateStatus();
	UpdateData(FALSE);
}

void CommandEditDialog::OnButtonBrowseFile1Clicked()
{
	UpdateData();
	CFileDialog dlg(TRUE, NULL, mPath, OFN_FILEMUSTEXIST, _T("All files|*.*||"));
	if (dlg.DoModal() != IDOK) {
		return;
	}

	mPath = dlg.GetPathName();
	UpdateStatus();
	UpdateData(FALSE);
}

void CommandEditDialog::OnButtonBrowseDir1Clicked()
{
	UpdateData();
	CFolderDialog dlg(_T(""), mPath);

	if (dlg.DoModal() != IDOK) {
		return;
	}

	mPath = dlg.GetPathName();
	UpdateStatus();
	UpdateData(FALSE);
}

void CommandEditDialog::OnButtonBrowseFile2Clicked()
{
	UpdateData();
	CFileDialog dlg(TRUE, NULL, mPath0, OFN_FILEMUSTEXIST, _T("All files|*.*||"));
	if (dlg.DoModal() != IDOK) {
		return;
	}

	mPath0 = dlg.GetPathName();
	UpdateStatus();
	UpdateData(FALSE);
}

void CommandEditDialog::OnButtonBrowseDir2Clicked()
{
	UpdateData();
	CFolderDialog dlg(_T(""), mPath0);

	if (dlg.DoModal() != IDOK) {
		return;
	}

	mPath0 = dlg.GetPathName();
	UpdateStatus();
	UpdateData(FALSE);
}

void CommandEditDialog::OnButtonBrowseDir3Clicked()
{
	UpdateData();
	CFolderDialog dlg(_T(""), mDir);

	if (dlg.DoModal() != IDOK) {
		return;
	}

	mDir = dlg.GetPathName();
	UpdateStatus();
	UpdateData(FALSE);
}

void CommandEditDialog::OnCheckUse0()
{
	UpdateData();
	UpdateStatus();
	UpdateData(FALSE);
}

HBRUSH CommandEditDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH br = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_STATUSMSG) {
		COLORREF crTxt = mMessage.IsEmpty() ? RGB(0,0,0) : RGB(255, 0, 0);
		pDC->SetTextColor(crTxt);
	}

	return br;
}

void CommandEditDialog::OnOK()
{
	UpdateData();
	if (UpdateStatus() == false) {
		return ;
	}

	__super::OnOK();
}