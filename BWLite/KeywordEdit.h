#pragma once

class KeywordEdit : public CEdit
{
public:
	KeywordEdit(CWnd* pParent = nullptr);	// $BI8=`%3%s%9%H%i%/%?!<(B
	virtual ~KeywordEdit();

	void SetCaretToEnd();

protected:
	//virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV $B%5%]!<%H(B

// $B<BAu(B
protected:
	afx_msg void OnKeyDown(UINT,UINT,UINT);
	afx_msg UINT OnGetDlgCode();
	DECLARE_MESSAGE_MAP()
};
