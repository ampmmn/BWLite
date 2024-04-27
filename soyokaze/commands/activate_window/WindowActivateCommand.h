#pragma once

#include "commands/common/UserCommandBase.h"
#include <memory>

namespace launcherapp {
namespace commands {
namespace activate_window {

class WindowActivateCommand : public launcherapp::commands::common::UserCommandBase
{
public:
	WindowActivateCommand();
	virtual ~WindowActivateCommand();

	CString GetName() override;
	CString GetDescription() override;
	CString GetGuideString() override;
	CString GetTypeDisplayName() override;

	BOOL Execute(const Parameter& param) override;
	CString GetErrorString() override;
	HICON GetIcon() override;
	int Match(Pattern* pattern) override;
	int EditDialog(const Parameter* param) override;
	bool IsPriorityRankEnabled() override;
	launcherapp::core::Command* Clone() override;

	bool Save(CommandFile* cmdFile) override;

	static CString GetType();

	static bool NewDialog(const Parameter* param, WindowActivateCommand** newCmd);
	static bool LoadFrom(CommandFile* cmdFile, void* entry, WindowActivateCommand** newCmdPtr);

protected:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};


} // end of namespace activate_window
} // end of namespace commands
} // end of namespace launcherapp

