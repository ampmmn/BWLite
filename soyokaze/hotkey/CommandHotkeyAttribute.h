#pragma once

#include "hotkey/HotKeyAttribute.h"

// $B%3%^%s%IMQ$N%[%C%H%-!<B0@-(B
class CommandHotKeyAttribute : public HOTKEY_ATTR
{
public:
	CommandHotKeyAttribute(bool isGlobal = false);
	CommandHotKeyAttribute(const CommandHotKeyAttribute& rhs);
	CommandHotKeyAttribute(UINT modifiers, UINT hotkey, bool isGlobal = false);

	bool operator == (const CommandHotKeyAttribute& rhs) const;
	bool operator != (const CommandHotKeyAttribute& rhs) const;
	bool operator < (const CommandHotKeyAttribute& rhs) const;

	CommandHotKeyAttribute& operator = (const CommandHotKeyAttribute& rhs);

	bool IsGlobal() const;

	bool mIsGlobal;
};



