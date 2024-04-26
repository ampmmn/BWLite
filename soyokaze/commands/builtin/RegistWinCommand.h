#pragma once

#include "commands/builtin/BuiltinCommandBase.h"

namespace launcherapp {
namespace commands {
namespace builtin {

class RegistWinCommand : public BuiltinCommandBase
{
public:
	RegistWinCommand(LPCTSTR name = nullptr);
	virtual ~RegistWinCommand();

	BOOL Execute(const Parameter& param) override;
	HICON GetIcon() override;
	launcherapp::core::Command* Clone() override;

	CString GetType() override;
	static CString TYPE;

	// BuiltinCommandFactory$B7PM3$G%$%s%9%?%s%9$r@8@.$G$-$k$h$&$K$9$k$?$a$N<jB3$-(B
	static launcherapp::core::Command* Create(LPCTSTR);
	DECLARE_BUILTINCOMMAND(RegistWinCommand)
};

} // end of namespace builtin
} // end of namespace commands
} // end of namespace launcherapp

