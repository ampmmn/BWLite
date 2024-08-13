#pragma once

#include "commands/common/RoutineCommandProviderBase.h"

namespace launcherapp {
namespace commands {
namespace url_directoryindex {


class URLDirectoryIndexCommandProvider :
	public launcherapp::commands::common::RoutineCommandProviderBase
{

private:
	URLDirectoryIndexCommandProvider();
	~URLDirectoryIndexCommandProvider() override;

public:
	virtual CString GetName();

	// $B:n@.$G$-$k%3%^%s%I$N<oN`$rI=$9J8;zNs$r<hF@(B
	virtual CString GetDisplayName();

	// $B%3%^%s%I$N<oN`$N@bL@$r<($9J8;zNs$r<hF@(B
	virtual CString GetDescription();

	// $B%3%^%s%I?75,:n@.%@%$%"%m%0(B
	virtual bool NewDialog(const CommandParameter* param);

	// $B0l;~E*$J%3%^%s%I$rI,MW$K1~$8$FDs6!$9$k(B
	virtual void QueryAdhocCommands(Pattern* pattern, CommandQueryItemList& comands);

	// Provider$B4V$NM%@h=g0L$rI=$9CM$rJV$9!#>.$5$$$[$IM%@h(B
	virtual uint32_t GetOrder() const;

	DECLARE_COMMANDPROVIDER(URLDirectoryIndexCommandProvider)

// RoutineCommandProviderBase
	bool LoadFrom(CommandEntryIF* entry, Command** command) override;

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};


} // end of namespace filter
} // end of namespace commands
} // end of namespace launcherapp

