#pragma once

#include "commands/builtin/BuiltinCommandBase.h"

namespace launcherapp {
namespace commands {
namespace builtin {


class ReloadCommand : public BuiltinCommandBase
{
public:
	ReloadCommand(LPCTSTR name = nullptr);
	virtual ~ReloadCommand();

	BOOL Execute(const Parameter& param) override;
	HICON GetIcon() override;
	launcherapp::core::Command* Clone() override;

	CString GetType() override;
	static CString TYPE;
};

} // end of namespace builtin
} // end of namespace commands
} // end of namespace launcherapp

