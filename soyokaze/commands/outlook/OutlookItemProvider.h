#pragma once

#include "commands/common/AdhocCommandProviderBase.h"
#include "commands/core/CommandProviderIF.h"


namespace launcherapp {
namespace commands {
namespace outlook {

class OutlookItemProvider :
	public launcherapp::commands::common::AdhocCommandProviderBase
{
	using Command = launcherapp::core::Command;
	using CommandParameter = launcherapp::core::CommandParameter;

private:
	OutlookItemProvider();
	virtual ~OutlookItemProvider();

public:
	CString GetName() override;

	// $B0l;~E*$J%3%^%s%I$rI,MW$K1~$8$FDs6!$9$k(B
	void QueryAdhocCommands(Pattern* pattern, std::vector<CommandQueryItem>& comands) override;

	DECLARE_COMMANDPROVIDER(OutlookItemProvider)

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};


} // end of namespace outlook
} // end of namespace commands
} // end of namespace launcherapp

