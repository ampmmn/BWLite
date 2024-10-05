#pragma once

#include "commands/common/AdhocCommandBase.h"
#include <memory>

namespace launcherapp {
namespace commands {
namespace controlpanel {

class ControlPanelCommand : public launcherapp::commands::common::AdhocCommandBase
{
public:
	ControlPanelCommand(const CString& name, const CString& iconPath, const CString& command, const CString& description);
	virtual ~ControlPanelCommand();

	CString GetGuideString() override;
	CString GetTypeDisplayName() override;
	BOOL Execute(Parameter* param) override;
	HICON GetIcon() override;
	launcherapp::core::Command* Clone() override;

protected:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};


} // end of namespace controlpanel
} // end of namespace commands
} // end of namespace launcherapp

