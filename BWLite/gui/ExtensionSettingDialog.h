#pragma once

#include "gui/SettingPage.h"

// 
class ExtensionSettingDialog : public SettingPage
{
public:
	ExtensionSettingDialog(CWnd* parentWnd);
	virtual ~ExtensionSettingDialog();

protected:
	void OnOK() override;
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnInitDialog() override;
	BOOL OnKillActive() override;
	BOOL OnSetActive() override;

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

