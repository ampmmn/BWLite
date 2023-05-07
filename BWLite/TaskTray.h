#pragma once

class CBWLiteDlg;

// �^�X�N�g���C�ɕ\������E�C���h�E
class TaskTray : public CWnd
{
public:
	TaskTray(CBWLiteDlg* window);
	virtual ~TaskTray();

	DECLARE_DYNAMIC(TaskTray)

protected:
	HICON mIcon;
	HWND mTaskTrayWindow;
	CBWLiteDlg* mBWLiteWindowPtr;

public:
	BOOL Create();

	LRESULT OnNotifyTrakTray(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
	void OnContextMenu();
};

