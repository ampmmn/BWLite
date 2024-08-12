#pragma once

#include "commands/common/RoutineCommandProviderBase.h"

namespace launcherapp {
namespace commands {
namespace group {


// $B%0%k!<%W%3%^%s%I$r@8@.$9$k$?$a$N%/%i%9(B
class GroupCommandProvider :
	public launcherapp::commands::common::RoutineCommandProviderBase
{
private:
	GroupCommandProvider();
	~GroupCommandProvider() override;

public:
	virtual CString GetName();

	// $B:n@.$G$-$k%3%^%s%I$N<oN`$rI=$9J8;zNs$r<hF@(B
	virtual CString GetDisplayName();

	// $B%3%^%s%I$N<oN`$N@bL@$r<($9J8;zNs$r<hF@(B
	virtual CString GetDescription();

	// $B%3%^%s%I?75,:n@.%@%$%"%m%0(B
	virtual bool NewDialog(const CommandParameter* param);

	// Provider$B4V$NM%@h=g0L$rI=$9CM$rJV$9!#>.$5$$$[$IM%@h(B
	virtual uint32_t GetOrder() const;

	DECLARE_COMMANDPROVIDER(GroupCommandProvider)

// RoutineCommandProviderBase
	DECLARE_LOADFROM(GroupCommandProvider)
};

} // end of namespace group
} // end of namespace commands
} // end of namespace launcherapp

