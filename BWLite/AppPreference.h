#pragma once

class AppPreference
{
public:
	static AppPreference* Get();

	void Load();
	void Save();

	CString GetFilerPath() const;
	CString GetFilerParam() const;

protected:
	AppPreference();
	~AppPreference();

public:
	// $B%U%!%$%i!<$r;XDj$9$k$+(B?
	bool mIsUseExternalFiler;
	// $B%U%!%$%i!<(B
	CString mFilerPath;
	// $B%U%!%$%i!<<B9T;~$N0z?t(B
	CString mFilerParam;
	// $B%i%s%A%c!<I=<(MQ%[%C%H%-!<$N=$>~%-!<(B
	UINT mModifiers;
	// $B%i%s%A%c!<I=<(MQ%[%C%H%-!<(B
	UINT mHotKeyVK;

	// $BF)2aI=<($NF)L@EY(B(0-255)
	int mAlpha;
	// $BF)2aI=<($r$9$k$+(B
	bool mIsTransparencyEnable;
	// $BHs%"%/%F%#%V$N$H$-$@$1F)2aI=<((B(false$B"*>o$KF)2a(B)
	bool mIsTransparencyInactiveOnly;
	// $B%&%$%s%I%&$r:G>e0LI=<($9$k$+(B
	bool mIsTopmost;
	// $B8uJd9J$j9~$_J}K!(B
	int mMatchLevel;
};

