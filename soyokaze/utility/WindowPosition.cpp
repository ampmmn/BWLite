#include "pch.h"
#include "framework.h"
#include "WindowPosition.h"
#include "utility/AppProfile.h"
#include <utility> // for std::pair

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

WindowPosition::WindowPosition()
{
	DWORD bufLen = MAX_COMPUTERNAME_LENGTH + 1;
	BOOL ret = GetComputerName(mName.GetBuffer(bufLen), &bufLen);
	mName.ReleaseBuffer();

	if (ret == FALSE) {
		mName = _T("Window");
	}
}

WindowPosition::WindowPosition(LPCTSTR name)
{
	ASSERT(name);

	DWORD bufLen = MAX_COMPUTERNAME_LENGTH + 1;
	TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1];
	if (GetComputerName(computerName, &bufLen)) {
		mName = computerName;
		mName += _T(".");
		mName += name;
	}
	else {
		mName = name;
	}
}

WindowPosition::~WindowPosition()
{
	Save();
}

/**
 * $B%b%K%?Ns5s%3!<%k%P%C%/4X?t(B
 * $B%&%$%s%I%&$,%b%K%?$K<}$^$k$+$NH=Dj$r9T$&(B
 *
 * @param hm $B%b%K%?$N%O%s%I%k(B($B;H$o$J$$(B)
 * @param hdc $B%G%P%$%9%3%s%F%-%9%H(B($B;H$o$J$$(B)
 * @param rectMonitor $B%b%K%?NN0h(B
 * @param lp  $B%f!<%6%Q%i%a!<%?(B(std::pair<RECT,bool>)
 */
static BOOL CALLBACK MonitorCallback(HMONITOR hm, HDC hdc, LPRECT rectMonitor, LPARAM lp)
{
	std::pair<RECT,bool>* param = (std::pair<RECT,bool>*)lp;

	const RECT& rectWnd = param->first;
	bool& isOutOfMonitor = param->second;

	// $B%b%K%?NN0h$H%&%$%s%I%&NN0h$,8r:9$9$k$+$I$&$+$G<}$^$C$F$$$k$+$rH=CG$9$k(B
	RECT rectIntersect;
	if (IntersectRect(&rectIntersect, rectMonitor, &rectWnd)) {
		isOutOfMonitor = false;
	}

	return TRUE;
}

/**
 *  $B@_Dj%U%!%$%k(B(Soyokaze.position)$B$N>pJs$+$i%&%$%s%I%&0LCV$rI|85$9$k(B
 *  $BA02s$N0LCV$rI|85$9$k$?$a$K;HMQ$9$k(B
 *  $BI|85$7$?7k2L$N%&%$%s%I%&0LCV$,%b%K%?!<NN0h$K<}$^$C$F$$$J$$>l9g$O(Bfalse$B$rJV$9(B
 *
 *  @return true:$BI|85$7$?(B   false:$BI|85$7$J$+$C$?(B
 *  @param hwnd $BBP>]%&%$%s%I%&%O%s%I%k(B
 */
bool WindowPosition::Restore(HWND hwnd)
{
	if (IsWindow(hwnd) == FALSE) {
		return false;
	}

	TCHAR path[MAX_PATH_NTFS];
	GetFilePath(path, MAX_PATH_NTFS);


	if (PathFileExists(path) == FALSE) {
		// $B@_Dj%U%!%$%k$,B8:_$7$J$$>l9g$O5$5Y$a$K%G%U%)%k%H$N@_Dj%U%!%$%k$rN.MQ$9$k(B
		WindowPosition::GetFilePath(_T("Soyokaze"), path, MAX_PATH_NTFS);
	}

	CFile file;
	if (file.Open(path, CFile::modeRead | CFile::shareDenyWrite) == FALSE) {
		return false;
	}

	WINDOWPLACEMENT wp;
	file.Read(&wp, sizeof(wp));

	if (SetWindowPlacement(hwnd, &wp) == FALSE) {
		return FALSE;
	}

	// $B3F%b%K%?NN0h$NCf$KG<$^$C$F$$$k$+$r%A%'%C%/(B
	RECT rectWnd;
	GetWindowRect(hwnd, &rectWnd);

	std::pair<RECT, bool> param(rectWnd, true);
	EnumDisplayMonitors(NULL, NULL, MonitorCallback, (LPARAM)&param);

	return param.second == false;
}

bool WindowPosition::Update(HWND hwnd)
{
	WINDOWPLACEMENT wp;
	wp.length = sizeof(wp);
	if (GetWindowPlacement(hwnd, &wp) == false) {
		return false;
	}
	mPosition = wp;
	return true;
}

bool WindowPosition::Save()
{
	try {
		TCHAR path[MAX_PATH_NTFS];
		GetFilePath(path, MAX_PATH_NTFS);

		UINT nFlags = CFile::modeCreate | CFile::modeWrite | CFile::typeBinary;
		CFile file(path, nFlags);
		file.Write(&mPosition, sizeof(mPosition));
		return true;
	}
	catch(...) {
		return false;
	}
}


void WindowPosition::GetFilePath(TCHAR* pathBuf, size_t len)
{
	WindowPosition::GetFilePath(mName, pathBuf, len);
}

void WindowPosition::GetFilePath(LPCTSTR baseName, TCHAR* pathBuf, size_t len)
{
	CAppProfile::GetDirPath(pathBuf, len);
	PathAppend(pathBuf, baseName);
	_tcscat_s(pathBuf, len, _T(".position"));
}

