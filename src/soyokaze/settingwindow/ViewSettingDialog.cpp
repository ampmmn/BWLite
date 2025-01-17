#include "pch.h"
#include "framework.h"
#include "ViewSettingDialog.h"
#include "setting/Settings.h"
#include "app/Manual.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct ViewSettingDialog::PImpl
{
	// 入力欄のアイコンを表示する
	BOOL mIsDrawIcon = TRUE;
	// 入力欄のプレースホルダーを表示する
	BOOL mIsDrawPlaceHolder = TRUE;

	// 半透明の表示方法
	int mTransparencyType = 0;
	// 半透明表示の透明度
	UINT mAlpha = 128;

	// コマンド種別を表示するか?
	BOOL mIsShowCommandType = TRUE;

	// 操作ガイドを表示するか?
	BOOL mIsShowGuide = TRUE;

	// 候補欄の背景色を交互に変える
	BOOL mIsAlternateColor = TRUE;

	// 候補欄の各項目にアイコンを描画するか
	BOOL mIsDrawIconOnCandidate = TRUE;

	// 入力画面の初期状態時にコメント表示欄に表示する文字列
	CString mDefaultComment;
};

ViewSettingDialog::ViewSettingDialog(CWnd* parentWnd) : 
	SettingPage(_T("表示"), IDD_VIEWSETTING, parentWnd),
	in(std::make_unique<PImpl>())
{
}

ViewSettingDialog::~ViewSettingDialog()
{
}

BOOL ViewSettingDialog::OnKillActive()
{
	if (UpdateData() == FALSE) {
		return FALSE;
	}
	return TRUE;
}

BOOL ViewSettingDialog::OnSetActive()
{
	UpdateStatus();
	UpdateData(FALSE);
	return TRUE;
}

void ViewSettingDialog::OnOK()
{
	auto settingsPtr = (Settings*)GetParam();
	settingsPtr->Set(_T("ViewSetting:IsDrawIcon"), (bool)in->mIsDrawIcon);
	settingsPtr->Set(_T("ViewSetting:IsDrawPlaceHolder"), (bool)in->mIsDrawPlaceHolder);

	if (in->mTransparencyType == 0) {
		settingsPtr->Set(_T("WindowTransparency:Enable"), true);
		settingsPtr->Set(_T("WindowTransparency:InactiveOnly"), true);
	}
	else if (in->mTransparencyType == 1) {
		settingsPtr->Set(_T("WindowTransparency:Enable"), true);
		settingsPtr->Set(_T("WindowTransparency:InactiveOnly"), false);
	}
	else {
		settingsPtr->Set(_T("WindowTransparency:Enable"), false);
		settingsPtr->Set(_T("WindowTransparency:InactiveOnly"), true);
	}

	settingsPtr->Set(_T("WindowTransparency:Alpha"), (int)in->mAlpha);

	settingsPtr->Set(_T("Soyokaze:DefaultComment"), in->mDefaultComment);

	settingsPtr->Set(_T("Soyokaze:IsShowCommandType"), (bool)in->mIsShowCommandType);
	settingsPtr->Set(_T("Soyokaze:IsShowGuide"), (bool)in->mIsShowGuide);
	settingsPtr->Set(_T("Soyokaze:IsAlternateColor"), (bool)in->mIsAlternateColor);
	settingsPtr->Set(_T("Soyokaze:IsDrawIconOnCandidate"), (bool)in->mIsDrawIconOnCandidate);

	CMFCFontComboBox* fontCombo = (CMFCFontComboBox*)GetDlgItem(IDC_MFCFONTCOMBO_MAIN);
	ASSERT(fontCombo);
	auto fontInfo = fontCombo->GetSelFont();

	settingsPtr->Set(_T("MainWindow:FontName"), fontInfo->m_strName);


	__super::OnOK();
}

void ViewSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_DRAWICON, in->mIsDrawIcon);
	DDX_Check(pDX, IDC_CHECK_DRAWPLACEHOLDER, in->mIsDrawPlaceHolder);
	DDX_CBIndex(pDX, IDC_COMBO_TRANSPARENCY, in->mTransparencyType);
	DDX_Text(pDX, IDC_EDIT_ALPHA, in->mAlpha);
	DDV_MinMaxInt(pDX, in->mAlpha, 0, 255);
	DDX_Text(pDX, IDC_EDIT_DEFAULTCOMMENT, in->mDefaultComment);
	DDX_Check(pDX, IDC_CHECK_SHOWCOMMANDTYPE, in->mIsShowCommandType);
	DDX_Check(pDX, IDC_CHECK_SHOWGUIDE, in->mIsShowGuide);
	DDX_Check(pDX, IDC_CHECK_ALTERNATELISTCOLOR, in->mIsAlternateColor);
	DDX_Check(pDX, IDC_CHECK_DRAWICONONCANDIDATE, in->mIsDrawIconOnCandidate);
}

#pragma warning( push )
#pragma warning( disable : 26454 )

BEGIN_MESSAGE_MAP(ViewSettingDialog, SettingPage)
	ON_CBN_SELCHANGE(IDC_COMBO_TRANSPARENCY, OnCbnTransparencyChanged)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_MACRO, OnNotifyLinkOpen)
	ON_NOTIFY(NM_RETURN, IDC_SYSLINK_MACRO, OnNotifyLinkOpen)
END_MESSAGE_MAP()

#pragma warning( pop )

BOOL ViewSettingDialog::OnInitDialog()
{
	__super::OnInitDialog();

	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

bool ViewSettingDialog::UpdateStatus()
{
	GetDlgItem(IDC_EDIT_ALPHA)->EnableWindow(in->mTransparencyType != 2);

	return true;
}


void ViewSettingDialog::OnCbnTransparencyChanged()
{
	UpdateData();
	UpdateStatus();
}

void ViewSettingDialog::OnEnterSettings()
{
	auto settingsPtr = (Settings*)GetParam();

	in->mIsDrawIcon = settingsPtr->Get(_T("ViewSetting:IsDrawIcon"), true);
	in->mIsDrawPlaceHolder = settingsPtr->Get(_T("ViewSetting:IsDrawPlaceHolder"), true);

	if (settingsPtr->Get(_T("WindowTransparency:Enable"), false) == false) {
		in->mTransparencyType = 2;
	}
	else if (settingsPtr->Get(_T("WindowTransparency:InactiveOnly"), true)) {
		in->mTransparencyType = 0;
	}
	else {
		in->mTransparencyType = 1;
	}

	in->mAlpha = settingsPtr->Get(_T("WindowTransparency:Alpha"), 128);
	if (in->mAlpha < 0) { in->mAlpha = 0; }
	if (in->mAlpha > 255) { in->mAlpha = 255; }

	CString defStr((LPCTSTR)ID_STRING_DEFAULTDESCRIPTION);
	in->mDefaultComment = settingsPtr->Get(_T("Soyokaze:DefaultComment"), defStr);
	in->mIsShowCommandType = settingsPtr->Get(_T("Soyokaze:IsShowCommandType"), true);
	in->mIsShowGuide = settingsPtr->Get(_T("Soyokaze:IsShowGuide"), true);
	in->mIsAlternateColor = settingsPtr->Get(_T("Soyokaze:IsAlternateColor"), true);
	in->mIsDrawIconOnCandidate = settingsPtr->Get(_T("Soyokaze:IsDrawIconOnCandidate"), true);

	CString fontName = settingsPtr->Get(_T("MainWindow:FontName"), _T("Tahoma"));
	CMFCFontComboBox* fontCombo = (CMFCFontComboBox*)GetDlgItem(IDC_MFCFONTCOMBO_MAIN);
	ASSERT(fontCombo);
	fontCombo->SelectFont(fontName);
}

bool ViewSettingDialog::GetHelpPageId(CString& id)
{
	id = _T("InputWindowSetting");
	return true;
}

// マニュアル表示
void ViewSettingDialog::OnNotifyLinkOpen(
	NMHDR *pNMHDR,
 	LRESULT *pResult
)
{
	UNREFERENCED_PARAMETER(pNMHDR);

	auto manual = launcherapp::app::Manual::GetInstance();
	manual->Navigate(_T("MacroList"));
	*pResult = 0;
}


