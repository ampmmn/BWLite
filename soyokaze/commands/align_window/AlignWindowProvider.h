#pragma once

#include "core/CommandProviderIF.h"

namespace soyokaze {
namespace commands {
namespace align_window {

class AlignWindowProvider :
	public soyokaze::core::CommandProvider
{
	using Command = soyokaze::core::Command;
	using CommandParameter = soyokaze::core::CommandParameter;

private:
	AlignWindowProvider();
	virtual ~AlignWindowProvider();

public:
	// $B=i2s5/F0$N=i4|2=$r9T$&(B
	void OnFirstBoot() override;

	// $B%3%^%s%I$NFI$_9~$_(B
	void LoadCommands(CommandFile* commandFile) override;

	CString GetName() override;

	// $B:n@.$G$-$k%3%^%s%I$N<oN`$rI=$9J8;zNs$r<hF@(B
	CString GetDisplayName() override;

	// $B%3%^%s%I$N<oN`$N@bL@$r<($9J8;zNs$r<hF@(B
	CString GetDescription() override;

	// $B%3%^%s%I?75,:n@.%@%$%"%m%0(B
	bool NewDialog(const CommandParameter* param) override;

	// $BHs8x3+%3%^%s%I$+$I$&$+(B($B?75,:n@.BP>]$K$7$J$$(B)
	bool IsPrivate() const override;

	// $B0l;~E*$J%3%^%s%I$rI,MW$K1~$8$FDs6!$9$k(B
	void QueryAdhocCommands(Pattern* pattern, std::vector<CommandQueryItem>& comands) override;

	// Provider$B4V$NM%@h=g0L$rI=$9CM$rJV$9!#>.$5$$$[$IM%@h(B
	uint32_t GetOrder() const override;

	uint32_t AddRef() override;
	uint32_t Release() override;

	DECLARE_COMMANDPROVIDER(AlignWindowProvider)

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};


} // end of namespace align_window
} // end of namespace commands
} // end of namespace soyokaze
