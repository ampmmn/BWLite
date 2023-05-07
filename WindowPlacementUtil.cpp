#include "pch.h"
#include "framework.h"
#include "WindowPlacementUtil.h"
#include "AppProfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace util {
namespace window {

static LPCTSTR c_pszSection = _T("WindowPlacement");

/*!
 *	@brief �E�C���h�E�ʒu���̕ۑ�
 *	@return ������true
 *	@param[in] wnd �ΏۃE�C���h�E
 *	@param[in] key �ۑ��L�[
 */
bool SavePlacement(
	CWnd* wnd,
	LPCTSTR key
)
{
	ASSERT(key);
	if (wnd == NULL || wnd->GetSafeHwnd() == NULL) {
		return false;
	}

	WINDOWPLACEMENT wp;
	wp.length = sizeof(wp);
	GetWindowPlacement(wnd->GetSafeHwnd(), &wp);
	CAppProfile::Get()->WriteBinary(c_pszSection, key, &wp, wp.length);
	return true;
}

/*!
 *	@brief	�E�C���h�E�ʒu���̕���
 *	@return ������true
 *	@param[in] wnd �ΏۃE�C���h�E
 *	@param[in] key �ۑ��L�[
 */
bool LoadPlacement(
	CWnd* wnd,
	LPCTSTR key
)
{
	ASSERT(key);
	if (wnd == NULL || wnd->GetSafeHwnd() == NULL) {
		return false;
	}

	int len = (int)CAppProfile::Get()->GetBinary(c_pszSection, key, NULL, 0);
	if (len == 0) {
		return false;
	}

	WINDOWPLACEMENT wp;
	CAppProfile::Get()->GetBinary(c_pszSection, key, &wp, len);

	return SetWindowPlacement(wnd->GetSafeHwnd(), &wp) != FALSE;
}

}  // end of namespace window
}  // end of namespace util

