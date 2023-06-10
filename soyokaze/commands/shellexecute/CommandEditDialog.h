#pragma once

#include "utility/TopMostMask.h"
#include "HotKeyAttribute.h"

class IconLabel;

class CommandEditDialog : public CDialogEx
{
public:
	CommandEditDialog();
	virtual ~CommandEditDialog();

	void SetOrgName(const CString& name);
	void SetName(const CString& name);
	void SetPath(const CString& path);
	void SetDescription(const CString& desc);
	void SetParam(const CString& param);

	int GetShowType();
	void SetShowType(int type);

	bool UpdateStatus();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	void ResolveShortcut(CString& path);

	// 編集開始時のコマンド名
	CString mOrgName;

	// メッセージ欄
	CString mMessage;

public:
	// コマンド名
	CString mName;
	// 説明
	CString mDescription;
	// 管理者権限で実行
	BOOL mIsRunAsAdmin;
	// 表示方法
	int mShowType;
	// カレントディレクトリ
	CString mDir;
	// パス
	CString mPath;
	// パラメータ
	CString mParameter;
	// パス(引数なし版)
	CString mPath0;
	// パラメータ(引数なし版)
	CString mParameter0;
	// 引数なし版を使うか?
	BOOL mIsUse0;

	IconLabel* mIconLabelPtr;

	// ほっときー(ひょうじよう)
	CString mHotKey;
	HOTKEY_ATTR mHotKeyAttr;
	bool mIsGlobal;

private:
	TopMostMask mTopMostMask;
// 実装
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
	afx_msg void OnButtonHotKey();
	afx_msg void OnButtonResolveShortcut();
	afx_msg void OnButtonResolveShortcut0();
};

