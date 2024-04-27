#pragma once

#include "commands/builtin/BuiltinCommandBase.h"

namespace launcherapp {
namespace commands {
namespace builtin {


class DeleteCommand : public BuiltinCommandBase
{
public:
	DeleteCommand(LPCTSTR name = nullptr);
	virtual ~DeleteCommand();

	BOOL Execute(const Parameter& param) override;
	launcherapp::core::Command* Clone() override;

	CString GetType() override;
	static CString TYPE;

	// BuiltinCommandFactory経由でインスタンスを生成できるようにするための手続き
	DECLARE_BUILTINCOMMAND(DeleteCommand)
};

}
}
}
