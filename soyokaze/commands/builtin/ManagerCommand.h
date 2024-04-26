#pragma once

#include "commands/builtin/BuiltinCommandBase.h"

namespace launcherapp {
namespace commands {
namespace builtin {


class ManagerCommand : public BuiltinCommandBase
{
public:
	ManagerCommand(LPCTSTR name = nullptr);
	virtual ~ManagerCommand();

	BOOL Execute(const Parameter& param) override;
	HICON GetIcon() override;

	launcherapp::core::Command* Clone() override;

	CString GetType() override;
	static CString TYPE;

	// BuiltinCommandFactory$B7PM3$G%$%s%9%?%s%9$r@8@.$G$-$k$h$&$K$9$k$?$a$N<jB3$-(B
	static launcherapp::core::Command* Create(LPCTSTR);
	DECLARE_BUILTINCOMMAND(ManagerCommand)
};


}
}
}
