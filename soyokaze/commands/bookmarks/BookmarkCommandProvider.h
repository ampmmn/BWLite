#pragma once

#include "commands/common/AdhocCommandProviderBase.h"

namespace soyokaze {
namespace commands {
namespace bookmarks {

class BookmarkCommandProvider :
	public soyokaze::commands::common::AdhocCommandProviderBase
{
private:
	BookmarkCommandProvider();
	virtual ~BookmarkCommandProvider();

public:
	virtual CString GetName();

	// $B0l;~E*$J%3%^%s%I$rI,MW$K1~$8$FDs6!$9$k(B
	virtual void QueryAdhocCommands(Pattern* pattern, CommandQueryItemList& comands);

	// $B@_Dj%Z!<%8$r<hF@$9$k(B
	bool CreateSettingPages(CWnd* parent, std::vector<SettingPage*>& pages) override;

	DECLARE_COMMANDPROVIDER(BookmarkCommandProvider)

private:
	void QueryBookmarks(Pattern* pattern, CommandQueryItemList& comands);
	void QueryHistories(Pattern* pattern, CommandQueryItemList& comands);

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};


} // end of namespace bookmarks
} // end of namespace commands
} // end of namespace soyokaze

