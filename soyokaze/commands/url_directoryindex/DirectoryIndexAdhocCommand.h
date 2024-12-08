#pragma once

#include "commands/common/AdhocCommandBase.h"
#include "commands/core/ContextMenuSourceIF.h"
#include "commands/url_directoryindex/URLDirectoryIndexCommand.h"
#include "commands/url_directoryindex/DirectoryIndexQueryResult.h"
#include <memory>

namespace launcherapp {
namespace commands {
namespace url_directoryindex {

class CommandParam;

class DirectoryIndexAdhocCommand : public launcherapp::commands::common::AdhocCommandBase,
	virtual public launcherapp::commands::core::ContextMenuSource

{
public:
	DirectoryIndexAdhocCommand(URLDirectoryIndexCommand* baseCmd, const QueryResult& result);
	virtual ~DirectoryIndexAdhocCommand();

	CString GetName() override;
	CString GetDescription() override;
	CString GetGuideString() override;
	CString GetTypeDisplayName() override;
	BOOL Execute(Parameter* param) override;
	HICON GetIcon() override;
	launcherapp::core::Command* Clone() override;

// ContextMenuSource
	// $B%a%K%e!<$N9`L\?t$r<hF@$9$k(B
	int GetMenuItemCount() override;
	// $B%a%K%e!<$NI=<(L>$r<hF@$9$k(B
	bool GetMenuItemName(int index, LPCWSTR* displayNamePtr) override;
	// $B%a%K%e!<A*Br;~$N=hM}$r<B9T$9$k(B
	bool SelectMenuItem(int index, launcherapp::core::CommandParameter* param) override;

// UnknownIF
	bool QueryInterface(const launcherapp::core::IFID& ifid, void** cmd) override;

protected:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};




} // end of namespace url_directoryindex
} // end of namespace commands
} // end of namespace launcherapp


