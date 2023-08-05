#pragma once

#include "commands/common/AdhocCommandProviderBase.h"


namespace soyokaze {
namespace commands {
namespace activate_window {


class ActivateWindowProvider :
	public soyokaze::commands::common::AdhocCommandProviderBase
{
private:
	ActivateWindowProvider();
	virtual ~ActivateWindowProvider();

public:
	virtual CString GetName();

	// $B0l;~E*$J%3%^%s%I$rI,MW$K1~$8$FDs6!$9$k(B
	virtual void QueryAdhocCommands(Pattern* pattern, std::vector<CommandQueryItem>& comands);


	DECLARE_COMMANDPROVIDER(ActivateWindowProvider)

protected:
	// Excel$B%7!<%H@Z$jBX$(MQ%3%^%s%I@8@.(B
	void QueryAdhocCommandsForWorksheets(Pattern* pattern, std::vector<CommandQueryItem>& commands);
	// $B%&%$%s%I%&@Z$jBX$(MQ%3%^%s%I@8@.(B
	void QueryAdhocCommandsForWindows(Pattern* pattern, std::vector<CommandQueryItem>& commands);

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};


} // end of namespace activate_window
} // end of namespace commands
} // end of namespace soyokaze

