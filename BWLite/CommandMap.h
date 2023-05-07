#pragma once

#include "CommandIF.h"
#include <map>
#include <vector>

class CommandMap
{
public:
	CommandMap();
	CommandMap(const CommandMap& rhs);
	~CommandMap();

	void Clear();

	bool Has(const CString& name) const;
	Command* Get(const CString& name);

	// $BEPO?(B/$B2r=|(B
	void Register(Command* cmd);
	bool Unregister(Command* cmd);
	bool Unregister(const CString& name);

	void Swap(CommandMap& rhs);

	void Query(Pattern* pattern, std::vector<Command*>& commands);

	// $B:G=i$K8+$D$1$?MWAG$rJV$9(B
	Command* FindOne(Pattern* pattern);

	// $BG[Ns2=$9$k(B
	std::vector<Command*>& Enumerate(std::vector<Command*>& commands);

protected:
	std::map<CString, Command*> mMap;
};
