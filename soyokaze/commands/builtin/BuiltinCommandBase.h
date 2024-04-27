#pragma once

#include "commands/core/CommandIF.h"
#include "commands/builtin/BuiltinCommandFactory.h"

namespace launcherapp {
namespace commands {
namespace builtin {

class BuiltinCommandBase : public launcherapp::core::Command
{
public:
	using Entry = BuiltinCommandFactory::Entry;
public:
	BuiltinCommandBase(LPCTSTR name = nullptr);
	virtual ~BuiltinCommandBase();

	CString GetName() override;
	CString GetDescription() override;
	CString GetGuideString() override;
	CString GetTypeDisplayName() override;
	//BOOL Execute(const Parameter& param) override;
	CString GetErrorString() override;
	HICON GetIcon() override;
	int Match(Pattern* pattern) override;
	bool IsEditable() override;
	bool IsDeletable() override;
	int EditDialog(const Parameter* param) override;
	bool IsPriorityRankEnabled() override;
	//launcherapp::core::Command* Clone() override;

	bool Save(CommandFile* cmdFile) override;

	uint32_t AddRef() override;
	uint32_t Release() override;


	virtual CString GetType() = 0;
	virtual void LoadFrom(Entry* entry);

protected:
	CString mName;
	CString mDescription;
	CString mError;
	uint32_t mRefCount;

	// $B<B9TA0$K3NG'$9$k$+(B?
	bool mIsConfirmBeforeRun = false;
	// $B<B9TA0$N3NG'M-L5$rA*Br2DG=$+(B?
	bool mCanSetConfirm = false;

	// $B5!G=$OM-8z$+(B?
	bool mIsEnable = true;
	// $B5!G=$rL58z2=$G$-$k$+(B?
	bool mCanDisable = false;
};

}
}
}

