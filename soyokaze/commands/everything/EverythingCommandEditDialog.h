#pragma once

#include "gui/SinglePageDialog.h"
#include <memory>
#include "commands/everything/EverythingCommandParam.h"
#include "hotkey/HotKeyAttribute.h"

namespace launcherapp {
namespace commands {
namespace everything {

class SettingDialog : public launcherapp::gui::SinglePageDialog
{
public:
	SettingDialog();
	virtual ~SettingDialog();

	void SetParam(const CommandParam& param);
	const CommandParam& GetParam() const;

	void SetHotKeyAttribute(const HOTKEY_ATTR& attr, bool isGlobal);
	void GetHotKeyAttribute(HOTKEY_ATTR& attr, bool& isGlobal);

	bool UpdateStatus();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	struct PImpl;
	std::unique_ptr<PImpl> in;

// 実装
protected:
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnUpdateStatus();
	afx_msg void OnButtonBaseDir();
	afx_msg void OnButtonHotKey();
	afx_msg void OnNotifyLinkOpen(NMHDR *pNMHDR, LRESULT *pResult);
};

} // end of namespace everything
} // end of namespace commands
} // end of namespace launcherapp

