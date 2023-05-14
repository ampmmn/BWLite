﻿
// BWLiteDlg.h : ヘッダー ファイル
//

#pragma once

#include <vector>
#include "gui/KeywordEdit.h"
#include "gui/CmdReceiveEdit.h"
#include "gui/IconLabel.h"
#include "gui/BWLiteDropTarget.h"

class CommandRepository;
class Command;

class SharedHwnd;
class ExecHistory;
class HotKey;
class WindowPosition;
class WindowTransparency;

// CBWLiteDlg ダイアログ
class CBWLiteDlg : public CDialogEx
{
// コンストラクション
public:
	CBWLiteDlg(CWnd* pParent = nullptr);	// 標準コンストラクター
	virtual ~CBWLiteDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BWLITE_DIALOG };
#endif

public:
	static void ActivateWindow(HWND hwnd);
	void ActivateWindow();
	void HideWindow();
	void ShowHelp();

	bool ExecuteCommand(const CString& commandStr);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

	CommandRepository* GetCommandRepository();
	void SetDescription(const CString& msg);
	void ClearContent();

	// 現在選択中のコマンドを取得
	Command* GetCurrentCommand();


// 実装
protected:
	HICON m_hIcon;

	// キーワード入力欄の文字列
	CString mCommandStr;
	// 現在選択中のコマンドの説明
	CString m_strDescription;

	// 現在の候補
	std::vector<Command*> mCandidates;


	// コマンド管理マップ
	CommandRepository* m_pCommandRepository;
	// 選択中の候補
	int m_nSelIndex;

	// コマンド実行履歴
	ExecHistory* mExecHistory;

	// ウインドウハンドル(共有メモリに保存する用)
	SharedHwnd* m_pSharedHwnd;
	   // 後で起動したプロセスから有効化するために共有メモリに保存している

	// 候補一覧表示用リストボックス
	CListBox mCandidateListBox;
	// キーワード入力エディットボックス
	KeywordEdit mKeywordEdit;
	DWORD mLastCaretPos;

	// 外部からのコマンド受付用エディットボックス
	CmdReceiveEdit mCmdReceiveEdit;

	// アイコン描画用ラベル
	IconLabel mIconLabel;
	//
	HotKey* mHotKeyPtr;
	// ウインドウ位置を保存するためのクラス
	WindowPosition* mWindowPositionPtr;
	// ウインドウの透明度を制御するためのクラス
	WindowTransparency* mWindowTransparencyPtr;

	// ドロップターゲット
	BWLiteDropTarget mDropTargetDialog;
	BWLiteDropTarget mDropTargetEdit;

	// 生成された、メッセージ割り当て関数
	BOOL OnInitDialog() override;
	void OnOK() override;
	void OnCancel() override;
	LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp) override;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnEditCommandChanged();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLbnSelChange();
	afx_msg void OnLbnDblClkCandidate();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnActivate(UINT, CWnd* wnd, BOOL bActive);
	// コンテキストメニューの表示
	LRESULT OnKeywordEditNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserMessageActiveWindow(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserMessageSetText(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserMessageDragOverObject(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserMessageDropObject(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);
};