#pragma once

#include "commands/common/AdhocCommandBase.h"
#include <memory>

namespace soyokaze {
namespace commands {
namespace activate_window {

class CalcWorksheet;

class CalcWorksheetCommand : public soyokaze::commands::common::AdhocCommandBase
{
public:
	CalcWorksheetCommand(CalcWorksheet* sheet);
	virtual ~CalcWorksheetCommand();

	CString GetTypeDisplayName() override;
	BOOL Execute(const Parameter& param) override;
	HICON GetIcon() override;
	soyokaze::core::Command* Clone() override;

protected:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};


} // end of namespace activate_window
} // end of namespace commands
} // end of namespace soyokaze

