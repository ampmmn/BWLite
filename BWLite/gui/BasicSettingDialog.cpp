#include "pch.h"
#include "framework.h"
#include "BasicSettingDialog.h"
#include "gui/ShortcutDialog.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BasicSettingDialog::BasicSettingDialog(CWnd* parentWnd) : 
	SettingPage(_T("基本"), IDD_BASICSETTING, parentWnd)
{
}

BasicSettingDialog::~BasicSettingDialog()
{
}

BOOL BasicSettingDialog::OnKillActive()
{
	return TRUE;
}

BOOL BasicSettingDialog::OnSetActive()
{
	mHotKeyAttr = HOTKEY_ATTR(mSettingsPtr->Get(_T("HotKey:Modifiers"), 5),
		                        mSettingsPtr->Get(_T("HotKey:VirtualKeyCode"), 112));
	mHotKey = mHotKeyAttr.ToString();

	mIsShowToggle = mSettingsPtr->Get(_T("BWLite:ShowToggle"), false);
	mMatchLevel = mSettingsPtr->Get(_T("BWLite:MatchLevel"), 1);
	mIsUseExternalFiler = mSettingsPtr->Get(_T("BWLite:UseFiler"), false);
	mFilerPath = mSettingsPtr->Get(_T("BWLite:FilerPath"), _T(""));
	mFilerParam = mSettingsPtr->Get(_T("BWLite:FilerParam"), _T(""));
	mIsTopMost = mSettingsPtr->Get(_T("BWLite:TopMost"), false);

	if (mSettingsPtr->Get(_T("WindowTransparency:Enable"), false) == false) {
		mTransparencyType = 2;
	}
	else if (mSettingsPtr->Get(_T("WindowTransparency:InactiveOnly"), true)) {
		mTransparencyType = 0;
	}
	else {
		mTransparencyType = 1;
	}

	mAlpha = mSettingsPtr->Get(_T("WindowTransparency:Alpha"), 128);

	UpdateStatus();
	UpdateData(FALSE);
	return TRUE;
}

void BasicSettingDialog::OnOK()
{
	UpdateData();

	mSettingsPtr->Set(_T("HotKey:Modifiers"), (int)mHotKeyAttr.GetModifiers());
	mSettingsPtr->Set(_T("HotKey:VirtualKeyCode"), (int)mHotKeyAttr.GetVKCode());
	mSettingsPtr->Set(_T("BWLite:ShowToggle"), (bool)mIsShowToggle);
	mSettingsPtr->Set(_T("BWLite:MatchLevel"), mMatchLevel);
	mSettingsPtr->Set(_T("BWLite:UseFiler"), (bool)mIsUseExternalFiler);
	mSettingsPtr->Set(_T("BWLite:FilerPath"), mFilerPath);
	mSettingsPtr->Set(_T("BWLite:FilerParam"), mFilerParam);
	mSettingsPtr->Set(_T("BWLite:TopMost"), (bool)mIsTopMost);

	if (mTransparencyType == 0) {
		mSettingsPtr->Set(_T("WindowTransparency:Enable"), true);
		mSettingsPtr->Set(_T("WindowTransparency:InactiveOnly"), true);
	}
	else if (mTransparencyType == 1) {
		mSettingsPtr->Set(_T("WindowTransparency:Enable"), true);
		mSettingsPtr->Set(_T("WindowTransparency:InactiveOnly"), false);
	}
	else {
		mSettingsPtr->Set(_T("WindowTransparency:Enable"), false);
		mSettingsPtr->Set(_T("WindowTransparency:InactiveOnly"), true);
	}

	mSettingsPtr->Set(_T("WindowTransparency:Alpha"), (int)mAlpha);

	__super::OnOK();
}

void BasicSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_HOTKEY, mHotKey);
	DDX_Check(pDX, IDC_CHECK_SHOWTOGGLE, mIsShowToggle);
	DDX_CBIndex(pDX, IDC_COMBO_MATCHLEVEL, mMatchLevel);
	DDX_Check(pDX, IDC_CHECK_USEFILER, mIsUseExternalFiler);
	DDX_Text(pDX, IDC_EDIT_FILERPATH, mFilerPath);
	DDX_Text(pDX, IDC_EDIT_FILERPARAM, mFilerParam);
	DDX_Check(pDX, IDC_CHECK_TOPMOST, mIsTopMost);
	DDX_CBIndex(pDX, IDC_COMBO_TRANSPARENCY, mTransparencyType);
	DDX_Text(pDX, IDC_EDIT_ALPHA, mAlpha);
	DDV_MinMaxInt(pDX, mAlpha, 0, 255);
}

BEGIN_MESSAGE_MAP(BasicSettingDialog, SettingPage)
	ON_COMMAND(IDC_BUTTON_HOTKEY, OnButtonHotKey)
	ON_COMMAND(IDC_BUTTON_BROWSEFILE, OnButtonBrowseFile)
	ON_COMMAND(IDC_BUTTON_SHORTCUT, OnButtonShortcut)
	ON_COMMAND(IDC_CHECK_USEFILER, OnCheckUseFilter)
	ON_CBN_SELCHANGE(IDC_COMBO_TRANSPARENCY, OnCbnTransparencyChanged)
END_MESSAGE_MAP()


BOOL BasicSettingDialog::OnInitDialog()
{
	__super::OnInitDialog();

	// ファイル選択ボタン(絵文字にする)
	GetDlgItem(IDC_BUTTON_BROWSEFILE)->SetWindowTextW(L"\U0001F4C4");

	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

bool BasicSettingDialog::UpdateStatus()
{
	mHotKey = mHotKeyAttr.ToString();

	GetDlgItem(IDC_EDIT_FILERPATH)->EnableWindow(mIsUseExternalFiler);
	GetDlgItem(IDC_EDIT_FILERPARAM)->EnableWindow(mIsUseExternalFiler);
	GetDlgItem(IDC_BUTTON_BROWSEFILE)->EnableWindow(mIsUseExternalFiler);

	GetDlgItem(IDC_EDIT_ALPHA)->EnableWindow(mTransparencyType != 2);

	return true;
}

void BasicSettingDialog::OnButtonHotKey()
{
	UpdateData();

	HotKeyDialog dlg(mHotKeyAttr);
	if (dlg.DoModal() != IDOK) {
		return ;
	}

	dlg.GetAttribute(mHotKeyAttr);

	UpdateStatus();
	UpdateData(FALSE);
}

void BasicSettingDialog::OnButtonBrowseFile()
{
	UpdateData();

	CString filterStr((LPCTSTR)IDS_FILTER_EXE);
	CFileDialog dlg(TRUE, NULL, mFilerPath, OFN_FILEMUSTEXIST, filterStr, this);
	if (dlg.DoModal() != IDOK) {
		return;
	}

	mFilerPath = dlg.GetPathName();
	UpdateStatus();
	UpdateData(FALSE);
}

void BasicSettingDialog::OnCheckUseFilter()
{
	UpdateData();
	UpdateStatus();
}

void BasicSettingDialog::OnCbnTransparencyChanged()
{
	UpdateData();
	UpdateStatus();
}

void BasicSettingDialog::OnButtonShortcut()
{
	ShortcutDialog dlg;
	dlg.DoModal();
}


