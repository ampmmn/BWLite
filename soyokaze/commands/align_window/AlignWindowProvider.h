#pragma once

#include "commands/common/RoutineCommandProviderBase.h"

namespace launcherapp {
namespace commands {
namespace align_window {

class AlignWindowProvider :
	public launcherapp::commands::common::RoutineCommandProviderBase
{
private:
	AlignWindowProvider();
	virtual ~AlignWindowProvider();

public:
	CString GetName() override;

	// $B:n@.$G$-$k%3%^%s%I$N<oN`$rI=$9J8;zNs$r<hF@(B
	CString GetDisplayName() override;

	// $B%3%^%s%I$N<oN`$N@bL@$r<($9J8;zNs$r<hF@(B
	CString GetDescription() override;

	// $B%3%^%s%I?75,:n@.%@%$%"%m%0(B
	bool NewDialog(const CommandParameter* param) override;

	// Provider$B4V$NM%@h=g0L$rI=$9CM$rJV$9!#>.$5$$$[$IM%@h(B
	uint32_t GetOrder() const override;

	DECLARE_COMMANDPROVIDER(AlignWindowProvider)

	DECLARE_LOADFROM(AlignWindowProvider)
};


} // end of namespace align_window
} // end of namespace commands
} // end of namespace launcherapp

