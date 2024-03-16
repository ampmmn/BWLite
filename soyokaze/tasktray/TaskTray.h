#pragma once

#include "tasktray/TaskTrayEventListenerIF.h"

class TaskTray : public CWnd
{
public:
	TaskTray(TaskTrayEventListenerIF* listener);
	virtual ~TaskTray();

	DECLARE_DYNAMIC(TaskTray)

	void ShowMessage(const CString& msg);
	void ShowMessage(const CString& msg, const CString& title);

protected:
	// $B%?%9%/%H%l%$%"%$%3%s(B
	HICON mIcon;
	// $B%?%9%/%H%l%$%&%$%s%I%&(B
	HWND mTaskTrayWindow;
	// $B%?%9%/%H%l%$4XO"%$%Y%s%HDLCN@h(B
	TaskTrayEventListenerIF* mListenerPtr;

	// $B%?%9%/%H%l%$$KEPO?MQ$N>pJs(B
	NOTIFYICONDATA mNotifyIconData;

public:
	BOOL Create();

	LRESULT OnNotifyTrakTray(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
	void OnContextMenu();
};

