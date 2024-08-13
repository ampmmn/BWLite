#include "pch.h"
#include "URLDirectoryIndexCommandParam.h"
#include <array>

namespace launcherapp {
namespace commands {
namespace url_directoryindex {

CommandParam::CommandParam()
{
}

CommandParam::CommandParam(const CommandParam& rhs) : 
	mName(rhs.mName),
	mDescription(rhs.mDescription),
	mURL(rhs.mURL),
	mHotKeyAttr(rhs.mHotKeyAttr)
{
}

CommandParam::~CommandParam()
{
}

CommandParam& CommandParam::operator = (const CommandParam& rhs)
{
	if (this != &rhs) {
		mName = rhs.mName;
		mDescription = rhs.mDescription;
		mURL = rhs.mURL;
		mHotKeyAttr = rhs.mHotKeyAttr;
	}
	return *this;

}

// $B%5%V%Q%9$rO"7k(B
CString CommandParam::CombineURL(const CString& subPath) const
{
	// FIXME: subPath$B$,(B"/"$B$G;O$^$k>l9g$O!"%k!<%H$+$i$N%Q%9$K$9$k(B

	CString url(mURL);
	if (url.Right(1) != _T('/')) {
		url += _T('/');
	}
	url += subPath;

	return url;
}

bool CommandParam::IsContentURL(const CString& url) const
{
	CString ext = PathFindExtension(url);
	if (ext.IsEmpty()) {
		return false;
	}

	static std::array<LPCTSTR, 2> exts { _T(".hpi"), _T(".zip") };
	for (auto e : exts) {
		if (ext.CompareNoCase(e) != 0) {
			continue;
		}
		return true;
	}
	return false;
}

} // end of namespace url_directoryindex
} // end of namespace commands
} // end of namespace launcherapp

