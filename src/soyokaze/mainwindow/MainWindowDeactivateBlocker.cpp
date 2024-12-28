#include "pch.h"
#include "MainWindowDeactivateBlocker.h"
#include "SharedHwnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


MainWindowDeactivateBlocker::MainWindowDeactivateBlocker()
{
	// $B%U%)!<%+%9$r<:$C$?$H$-$K1#$l$k$N$rAK32$9$k(B
	SharedHwnd mainWnd;
	SendMessage(mainWnd.GetHwnd(), WM_APP + 14, 0, 1);

}

MainWindowDeactivateBlocker::~MainWindowDeactivateBlocker()
{
	// $B>uBV$r$b$H$KLa$9(B
	SharedHwnd mainWnd;
	SendMessage(mainWnd.GetHwnd(), WM_APP + 14, 0, 0);
}
