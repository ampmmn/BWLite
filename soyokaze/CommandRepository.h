#pragma once

#include "core/CommandIF.h"
#include "AppPreferenceListenerIF.h"
#include <vector>
#include <memory>

class CommandRepository : public AppPreferenceListenerIF
{
public:
	CommandRepository();
	virtual ~CommandRepository();

public:
	// $B%3%^%s%I%G!<%?$N%m!<%I(B
	BOOL Load();

	// $B?75,EPO?%@%$%"%m%0$NI=<((B
	int NewCommandDialog(const CString* cmdNamePtr, const CString* pathPtr, const CString* descStr = nullptr, const CString* paramStr = nullptr);
	// $B%3%^%s%IJT=8%@%$%"%m%0$NI=<((B
	int EditCommandDialog(const CString& cmdName);
	// $B%-!<%o!<%I%^%M!<%8%c%@%$%"%m%0$NI=<((B
	int ManagerDialog();
	// $B$^$H$a$FEPO?%@%$%"%m%0$NI=<((B
	int RegisterCommandFromFiles(const std::vector<CString>& files);

	bool DeleteCommand(const CString& cmdName);

	void EnumCommands(std::vector<soyokaze::core::Command*>& commands);

	bool IsBuiltinName(const CString& cmdName);

	void Query(const CString& strQueryStr, std::vector<soyokaze::core::Command*>& commands);
	soyokaze::core::Command* QueryAsWholeMatch(const CString& strQueryStr, bool isSearchPath = true);

	bool IsValidAsName(const CString& strQueryStr);

protected:
	virtual void OnAppPreferenceUpdated();

protected:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};

