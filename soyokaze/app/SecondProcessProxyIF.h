#pragma once


namespace launcherapp {

/**
 *  $B8eH/%W%m%;%9$,@h9T%W%m%;%9$KBP$7$F%3%^%s%I$rAw?.$9$k=hM}$H$7$F$N(BI/F
 */
class SecondProcessProxyIF
{
public:
	virtual ~SecondProcessProxyIF() {}

	// $B%3%^%s%IJ8;zNs$rDLCN$9$k(B
	virtual bool SendCommandString(const CString& commandStr, bool isPasteOnly) = 0;
	// $BA*BrHO0O$rDLCN$9$k(B
	virtual bool SendCaretRange(int startPos, int length) = 0;
	// $B%Q%9EPO?(B
	virtual bool RegisterPath(const CString& pathStr) = 0;
	// $B%+%l%s%H%G%#%l%/%H%j$rJQ99$9$k(B
	virtual bool ChangeDirectory(const CString& pathStr) = 0;
	// $B%&%$%s%I%&$r>C$9(B
	virtual bool Hide() = 0;
	// $B%&%$%s%I%&$rI=<($9$k(B
	virtual bool Show() = 0;
};

}

