#pragma once

namespace launcherapp {
namespace macros {
namespace core {

class Token
{
public:
	class Exception {};

public:
	Token(const CString& text) : mText(text), mPos(0)
	{
	}
	~Token() {}

	bool AtEnd()
	{
		return mPos == mText.GetLength();
	}

	void Next()
 	{
		if (AtEnd()) {
			return;
		}

		mPos++;
	}

	int GetPos()
	{
		return mPos;
	}

	TCHAR Get() {
		if (AtEnd()) {
			throw Exception(); 
		}
		return mText[mPos];
	}

	bool IsWhiteSpace()
	{
		if (AtEnd()) {
			return false;
		}
		return IsWhiteSpaceChar(Get());
	}

	void SkipWhiteSpace()
	{
		if (AtEnd()) {
			return;
		}

		TCHAR c = mText[mPos];

		while (IsWhiteSpaceChar(c)) {
			c = mText[++mPos];
			if (AtEnd()) {
				return;
			}
		}
	}

	void SkipName(CString& name)
 	{
		// $BL>A0$H$7$F;H$($kJ8;z(B
		static TCHAR NAMECHARS[] = _T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_");
		static size_t len = sizeof(NAMECHARS) / sizeof(NAMECHARS[0]);

		CString tmpName;

		while (AtEnd() == false) {
			TCHAR c = Get();
			auto it = std::find(NAMECHARS, NAMECHARS + len, c);
			if (it == NAMECHARS + len) {
				break;
			}
			tmpName += c;
			Next();
		}
		name = tmpName;
	}

	// c$B$,8=$l$k$^$G0LCV$r?J$a$k(B
	void SkipUntil(TCHAR c)
	{
		while (AtEnd() == false && Get() != c) {
			Next();
		}
	}

	// $B8=:_0LCV$K$"$kJ8;zNs$N<!$N0LCV$K?J$a$k!#Ht$P$7$?0LCV$K$"$kJ8;zNs$r(Bstr$B$KF~$l$FJV$9(B
	void SkipString(CString& str)
	{
		CString tmpStr;
		while (AtEnd() == false) {
			if (IsWhiteSpace() || Get() == _T('}')) {
				break;
			}
			if (Get() == _T('\\')) {
				// $B%(%9%1!<%W%7!<%1%s%9(B
				Next();
			}
			tmpStr += Get();
			Next();
		}
		str = tmpStr;
	}
	
	static bool IsWhiteSpaceChar(TCHAR c) {
		return c == _T(' ') || c == _T('\t');
	}

private:
	CString mText;
	int mPos;
};




}
}
}

