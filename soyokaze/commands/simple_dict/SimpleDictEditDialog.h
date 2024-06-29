#pragma once

#include "gui/SinglePageDialog.h"
#include <memory>
#include "commands/simple_dict/SimpleDictParam.h"
#include "hotkey/CommandHotKeyAttribute.h"

class ModalComboBox;

namespace launcherapp {
namespace commands {
namespace simple_dict {

class SettingDialog : public launcherapp::gui::SinglePageDialog
{
public:
	SettingDialog();
	virtual ~SettingDialog();

	void SetParam(const SimpleDictParam& param);
	const SimpleDictParam& GetParam() const;

	void SetHotKeyAttribute(const CommandHotKeyAttribute& attr);
	void GetHotKeyAttribute(CommandHotKeyAttribute& attr);

	bool UpdateStatus();

	bool Overlap(CWnd* dstWnd, CWnd* srcWnd);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	struct PImpl;
	std::unique_ptr<PImpl> in;

// 実装
protected:
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnUpdateName();
	afx_msg void OnUpdateCondition();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnUpdateStatus();
	afx_msg void OnButtonFilePath();
	afx_msg void OnButtonTest();
	afx_msg void OnButtonFrontRange();
	afx_msg void OnButtonBackRange();
	afx_msg void OnButtonBrowseAfterCommandFile();
	afx_msg void OnButtonBrowseAfterCommandDir();
	afx_msg void OnButtonHotKey();
	afx_msg void OnNotifyLinkOpen(NMHDR *pNMHDR, LRESULT *pResult);
};

} // end of namespace simple_dict
} // end of namespace commands
} // end of namespace launcherapp

