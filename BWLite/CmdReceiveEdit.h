#pragma once

class CmdReceiveEdit : public CEdit
{
public:
	CmdReceiveEdit(CWnd* pParent = nullptr);	// $BI8=`%3%s%9%H%i%/%?!<(B
	virtual ~CmdReceiveEdit();

// $B<BAu(B
protected:
	afx_msg int OnSetText(LPCTSTR text);
	DECLARE_MESSAGE_MAP()
};
