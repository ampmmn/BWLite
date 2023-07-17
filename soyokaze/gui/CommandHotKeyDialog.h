#pragma once

#include "HotKeyAttribute.h"

// 
class CommandHotKeyDialog : public CDialogEx
{
public:
	CommandHotKeyDialog(const HOTKEY_ATTR& attr);
	virtual ~CommandHotKeyDialog();

	void GetAttribute(HOTKEY_ATTR& attr);
	bool IsGlobal();

	HOTKEY_ATTR mHotKeyAttr;
	// $B=i4|CM(B
	HOTKEY_ATTR mHotKeyAttrInit;

	BOOL mIsGlobal;

	// $B%a%C%;!<%8Ms(B
	CString mMessage;

protected:
	static bool IsReservedKey(const HOTKEY_ATTR& attr);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV $B%5%]!<%H(B
	virtual BOOL OnInitDialog();

// $B<BAu(B
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void UpdateStatus();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonClear();
};

