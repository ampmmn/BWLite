#pragma once

#include "commands/core/CommandProviderIF.h"

namespace soyokaze {
namespace commands {
namespace watchpath {


class WatchPathCommandProvider :
	public soyokaze::core::CommandProvider
{
	using Command = soyokaze::core::Command;
	using CommandParameter = soyokaze::core::CommandParameter;

private:
	WatchPathCommandProvider();
	virtual ~WatchPathCommandProvider();

public:
	// $B=i2s5/F0$N=i4|2=$r9T$&(B
	virtual void OnFirstBoot();

	// $B%3%^%s%I$NFI$_9~$_(B
	virtual void LoadCommands(CommandFile* commandFile);

	virtual CString GetName();

	// $B:n@.$G$-$k%3%^%s%I$N<oN`$rI=$9J8;zNs$r<hF@(B
	virtual CString GetDisplayName();

	// $B%3%^%s%I$N<oN`$N@bL@$r<($9J8;zNs$r<hF@(B
	virtual CString GetDescription();

	// $B%3%^%s%I?75,:n@.%@%$%"%m%0(B
	virtual bool NewDialog(const CommandParameter* param);

	// $BHs8x3+%3%^%s%I$+$I$&$+(B($B?75,:n@.BP>]$K$7$J$$(B)
	virtual bool IsPrivate() const;

	// $B0l;~E*$J%3%^%s%I$rI,MW$K1~$8$FDs6!$9$k(B
	virtual void QueryAdhocCommands(Pattern* pattern, CommandQueryItemList& comands);

	// Provider$B4V$NM%@h=g0L$rI=$9CM$rJV$9!#>.$5$$$[$IM%@h(B
	virtual uint32_t GetOrder() const;

	// $B@_Dj%Z!<%8$r<hF@$9$k(B
	bool CreateSettingPages(CWnd* parent, std::vector<SettingPage*>& pages) override;

	virtual uint32_t AddRef();
	virtual uint32_t Release();

	DECLARE_COMMANDPROVIDER(WatchPathCommandProvider)

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};


}
}
}

