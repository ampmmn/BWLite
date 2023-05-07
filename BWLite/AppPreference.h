#pragma once

class AppPreference
{
public:
	AppPreference();
	~AppPreference();

	void Load();

	bool IsMatchCase() const;

	CString GetFilerPath() const;
	CString GetFilerParam() const;


public:
	// $B%-!<%o!<%I%^%C%A%s%0$GBgJ8;z>.J8;z$r6hJL$9$k(B
	bool mIsMatchCase;

	// $B%U%!%$%i!<(B
	CString mFilerPath;
	// $B%U%!%$%i!<<B9T;~$N0z?t(B
	CString mFilerParam;

};

