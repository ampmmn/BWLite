#pragma once

class AppPreference
{
public:
	AppPreference();
	~AppPreference();

	void Load();

	CString GetFilerPath() const;
	CString GetFilerParam() const;


public:
	// $B%U%!%$%i!<(B
	CString mFilerPath;
	// $B%U%!%$%i!<<B9T;~$N0z?t(B
	CString mFilerParam;

};

