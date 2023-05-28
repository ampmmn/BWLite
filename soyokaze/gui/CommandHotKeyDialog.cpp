#include "pch.h"
#include "framework.h"
#include "gui/CommandHotKeyDialog.h"
#include "core/CommandHotKeyManager.h"
#include "resource.h"
#include <utility>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CommandHotKeyDialog::CommandHotKeyDialog(const HOTKEY_ATTR& attr) : 
	CDialogEx(IDD_HOTKEY),
	mHotKeyAttr(attr),
	mIsGlobal(FALSE)
{
}

CommandHotKeyDialog::~CommandHotKeyDialog()
{
}

void CommandHotKeyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SHIFT, mHotKeyAttr.mUseShift);
	DDX_Check(pDX, IDC_CHECK_ALT, mHotKeyAttr.mUseAlt);
	DDX_Check(pDX, IDC_CHECK_CTRL, mHotKeyAttr.mUseCtrl);
	DDX_Check(pDX, IDC_CHECK_WIN, mHotKeyAttr.mUseWin);
	DDX_CBIndex(pDX, IDC_COMBO_VK, mHotKeyAttr.mVirtualKeyIdx);
	DDX_Text(pDX, IDC_STATIC_STATUSMSG, mMessage);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, mIsGlobal);
}

BEGIN_MESSAGE_MAP(CommandHotKeyDialog, CDialogEx)
	ON_COMMAND(IDC_CHECK_SHIFT, UpdateStatus)
	ON_COMMAND(IDC_CHECK_ALT, UpdateStatus)
	ON_COMMAND(IDC_CHECK_CTRL, UpdateStatus)
	ON_COMMAND(IDC_CHECK_WIN, UpdateStatus)
	ON_CBN_SELCHANGE(IDC_COMBO_VK, UpdateStatus)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, UpdateStatus)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void CommandHotKeyDialog::GetAttribute(HOTKEY_ATTR& attr)
{
	attr = mHotKeyAttr;
}

bool CommandHotKeyDialog::IsGlobal()
{
	return mIsGlobal != FALSE;
}

BOOL CommandHotKeyDialog::OnInitDialog()
{
	__super::OnInitDialog();

	// $B=i4|CM$r3P$($F$*$/(B
	mHotKeyAttrInit = mHotKeyAttr;

	UpdateStatus();

	return TRUE;
}

void CommandHotKeyDialog::UpdateStatus()
{
	UpdateData();

	GetDlgItem(IDC_CHECK_WIN)->EnableWindow(mIsGlobal);
	if (mIsGlobal == FALSE) {
		// $B%m!<%+%k%[%C%H%-!<(B($B"*%-!<%"%/%;%i%l!<%?(B)$B$N>l9g$O!"(BWIN$B%-!<$,;H$($J$$$N$G%A%'%C%/$r30$9(B
		mHotKeyAttr.mUseWin = 0;
	}

	mMessage.Empty();
	if (mHotKeyAttr != mHotKeyAttrInit) {

		// $B@_Dj$,=i4|CM$H0[$J$k>l9g$O!"$=$N%-!<$,;H$($k$+$I$&$+$r%A%'%C%/$9$k(B

		if (mIsGlobal) {
			bool canRegister = mHotKeyAttr.TryRegister(GetSafeHwnd());
			GetDlgItem(IDOK)->EnableWindow(canRegister);

			if (canRegister == false) {
				mMessage.LoadString(IDS_ERR_HOTKEYALREADYUSE);
			}
		}
		else {
			auto manager = soyokaze::core::CommandHotKeyManager::GetInstance();
			bool alreadUsed = manager->HasKeyBinding(mHotKeyAttr);
			if (alreadUsed) {
				mMessage.LoadString(IDS_ERR_HOTKEYALREADYUSE);
			}
		}
	}

	UpdateData(FALSE);
}

/**
 *  $B%(%i!<$N;~$K0lIt%3%s%H%m!<%k$N?'$rJQ$($k(B
 */
HBRUSH CommandHotKeyDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH br = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_STATUSMSG) {
		COLORREF crTxt = mMessage.IsEmpty() ? RGB(0,0,0) : RGB(255, 0, 0);
		pDC->SetTextColor(crTxt);
	}

	return br;
}
