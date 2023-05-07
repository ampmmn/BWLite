#include "pch.h"
#include "framework.h"
#include "Pattern.h"

CString Pattern::StripEscapeChars(const CString& pattern)
{
	// ��i��wregex�ɒl��n�����Ƃ��ɃG�X�P�[�v�L���Ƃ��ĉ��߂����̂�h������

	CString tmp(pattern);
	tmp.Replace(_T("\\"), _T(""));
	tmp.Replace(_T("?"), _T(""));
	tmp.Replace(_T("."), _T(""));
	tmp.Replace(_T("+"), _T(""));
	tmp.Replace(_T("^"), _T(""));
	tmp.Replace(_T("$"), _T(""));
	tmp.Replace(_T("["), _T(""));
	tmp.Replace(_T("]"), _T(""));
	tmp.Replace(_T("|"), _T(""));
	tmp.Replace(_T("("), _T(""));
	tmp.Replace(_T(")"), _T(""));
	tmp.Replace(_T("{"), _T(""));
	tmp.Replace(_T("}"), _T(""));
	tmp.Replace(_T("!"), _T(""));
	tmp.Replace(_T("&"), _T(""));

	return tmp;
}

