#pragma once

#include "commands/common/AdhocCommandProviderBase.h"

namespace soyokaze {
namespace commands {
namespace pathconvert {


class PathConvertProvider :
	public soyokaze::commands::common::AdhocCommandProviderBase
{
private:
	PathConvertProvider();
	virtual ~PathConvertProvider();

public:
	void AddHistory(const CString& word, const CString& fullPath);

public:
	// $B%3%^%s%I$NFI$_9~$_(B
	virtual void LoadCommands(CommandFile* commandFile);

	virtual CString GetName();
	// $B0l;~E*$J%3%^%s%I$rI,MW$K1~$8$FDs6!$9$k(B
	virtual void QueryAdhocCommands(Pattern* pattern, CommandQueryItemList& comands);

	// $B@_Dj%Z!<%8$r<hF@$9$k(B
	bool CreateSettingPages(CWnd* parent, std::vector<SettingPage*>& pages) override;

	DECLARE_COMMANDPROVIDER(PathConvertProvider)

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};


} // end of namespace pathconvert
} // end of namespace commands
} // end of namespace soyokaze

