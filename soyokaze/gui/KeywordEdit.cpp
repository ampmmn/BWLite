#include "pch.h"
#include "framework.h"
#include "KeywordEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

KeywordEdit::KeywordEdit(CWnd* pParent)
{
}

KeywordEdit::~KeywordEdit()
{
}

BEGIN_MESSAGE_MAP(KeywordEdit, CEdit)
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()

void KeywordEdit::SetCaretToEnd()
{
	int n = GetLimitText();
	SetSel(n, n, FALSE);
}

void KeywordEdit::OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags)
{

	if (nChar == VK_DOWN || nChar == VK_UP || nChar == VK_TAB) {

		// $B?F%&%$%s%I%&(B(CSoyokazeDlg)$B$K%-!<F~NO$rDLCN(B
		GetParent()->SendMessage(WM_APP + 1, nChar, 0);

		return ;
	}

	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}

UINT KeywordEdit::OnGetDlgCode()
{
	UINT ret = __super::OnGetDlgCode();

	const MSG* msg = CWnd::GetCurrentMessage();
	msg = (const MSG*)msg->lParam;

	// Tab$B%-!<$NF~NO$b(BWM_KEYDOWN$B$G=hM}$G$-$k$h$&$K$9$k(B
	if (msg && msg->message == WM_KEYDOWN && msg->wParam == VK_TAB) {
		ret |= DLGC_WANTMESSAGE;
	}

	return ret;
}

