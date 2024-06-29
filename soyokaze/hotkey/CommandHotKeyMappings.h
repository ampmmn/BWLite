#pragma once

#include <memory>

class CommandHotKeyAttribute;

class CommandHotKeyMappings
{
public:
	CommandHotKeyMappings();
	~CommandHotKeyMappings();

public:
	int GetItemCount() const;
	CString GetName(int index) const;
	void GetHotKeyAttr(int index, CommandHotKeyAttribute& hotKeyAttr) const;
	void AddItem(const CString& name, const CommandHotKeyAttribute& hotKeyAttr);
	void RemoveItem(const CommandHotKeyAttribute& hotKeyAttr);

	// $B%3%^%s%IL>$+$i3d$jEv$F%-!<$NI=<(MQJ8;zNs$r<hF@$9$k(B
	CString FindKeyMappingString(const CString& name) const;

	void Swap(CommandHotKeyMappings& rhs);


private:
	struct PImpl;
	std::unique_ptr<PImpl> in;

};

