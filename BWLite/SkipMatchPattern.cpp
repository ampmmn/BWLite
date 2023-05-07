#include "pch.h"
#include "framework.h"
#include "SkipMatchPattern.h"
#include <vector>
#include <regex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct SkipMatchPattern::PImpl
{
	std::wregex mRegPattern;
};

SkipMatchPattern::SkipMatchPattern() : in(new PImpl)
{
}

SkipMatchPattern::~SkipMatchPattern()
{
	delete in;
}

void SkipMatchPattern::SetPattern(
	const CString& pattern
)
{
	// 1$BJ8;z$4$H$K(B".*"$B$rIU$1$?$&$($G@55,I=8=%^%C%A%s%0$r$9$k$3$H$K$h$j!"(B
	// $B%9%-%C%W%^%C%A%s%0E*F0:n$r<B8=$9$k(B

	std::wstring pat;
	for (int i = 0; i < pattern.GetLength(); ++i) {
		pat += pattern[i];

		if (0xD800 <= pattern[i] && pattern[i] <= 0xDBFF) {
			// $B%5%m%2!<%H%Z%"$N@hF,(B1byte$B$O8eB3$N(B2$B%P%$%HL\$HIT2DJ,$N$?$a!"(B
			// $B!V(B.*$B!W$r$D$1$J$$(B
			continue;
		}

		pat += _T(".*");
	}
	in->mRegPattern = std::wregex(pat, std::regex_constants::icase);
}

bool SkipMatchPattern::Match(
	const CString& str
)
{
	return std::regex_search((const wchar_t*)str, in->mRegPattern);
}


