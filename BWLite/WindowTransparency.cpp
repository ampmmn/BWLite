#include "pch.h"
#include "framework.h"
#include "WindowTransparency.h"
#include "AppProfile.h"

WindowTransparency::WindowTransparency() : 
	mWindowHandle(nullptr),
	mAlpha(255),
	mIsEnable(true),
	mIsInactiveOnly(true)
{
	CAppProfile* app = CAppProfile::Get();
	mIsEnable = (app->Get(_T("WindowTransparency"), _T("Enable"), 1) != 0);

	mAlpha = app->Get(_T("WindowTransparency"), _T("Alpha"), 200);
	mIsInactiveOnly =(app->Get(_T("WindowTransparency"), _T("InactiveOnly"), 1) != 0);
}

WindowTransparency::~WindowTransparency()
{
}

void WindowTransparency::SetWindowHandle(HWND hwnd)
{
	mWindowHandle = hwnd;

	if (mIsEnable) {
		// WS_EX_LAYERED$B$rIUM?$9$k(B
		// ($B%@%$%"%m%0%(%G%#%?>e$G$3$N%9%?%$%k$r@_Dj$9$k$H%(%G%#%?>e$N%@%$%"%m%0$,9uEI$j$K$J$k$N$G(B)
		long exStyle = GetWindowLong(mWindowHandle, GWL_EXSTYLE);
		SetWindowLong(mWindowHandle, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);

		if (mIsInactiveOnly == false) {
			SetLayeredWindowAttributes(mWindowHandle, 0, mAlpha, LWA_ALPHA);
		}

	}
}

bool WindowTransparency::UpdateActiveState(UINT nState)
{
	if (IsWindow(mWindowHandle) == FALSE) {
		return false;
	}
	if (mIsEnable == false) {
		return false;
	}

	BYTE alpha = mAlpha;

	if (mIsInactiveOnly && nState != WA_INACTIVE) {
		alpha = 0xff;
	}

	SetLayeredWindowAttributes(mWindowHandle, 0, alpha, LWA_ALPHA);

	return true;
}

