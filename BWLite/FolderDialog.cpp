#include "pch.h"
#include "FolderDialog.h"

#if (defined(_DEBUG) && defined(WIN32))
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct CFolderDialogData
{
	CFolderDialogData() { ZeroMemory(szPath, sizeof(szPath));}
	CString strTitle;
	TCHAR szPath[MAX_PATH];
	HWND hwndParent;
};


/*=*************************************************************************//*!
	�R���X�g���N�^�B�����l��ݒ�B
	\param pszTitle			(I) �_�C�A���O�̃^�C�g��
	\param pszInitialPath	(I) �����t�H���_
	\param pwndParent		(I) �e�E�B���h�E
*//*=**************************************************************************/
CFolderDialog::CFolderDialog(
	LPCTSTR pszTitle,
	LPCTSTR pszInitialPath,
	CWnd *pwndParent
)
{
	Init(pszTitle, pszInitialPath, pwndParent);
}


/*=*************************************************************************//*!
	�R���X�g���N�^�B�����l��ݒ�B
	\param uTitleID			(I) �_�C�A���O�̃^�C�g���̕�����ID
	\param pszInitialPath	(I) �����t�H���_
	\param pwndParent		(I) �e�E�B���h�E
*//*=**************************************************************************/
CFolderDialog::CFolderDialog(
	UINT uTitleID,
	LPCTSTR pszInitialPath,
	CWnd *pwndParent
)
{
	CString strTitle;
	if (uTitleID > 0)
		strTitle.LoadString(uTitleID);

	Init(strTitle, pszInitialPath, pwndParent);
}

/*=*************************************************************************//*!
	�f�X�g���N�^�B�������̉��
*//*=**************************************************************************/
CFolderDialog::~CFolderDialog()
{
	delete m_pData;
}

/********************************************************
// ������
 ********************************************************/
void CFolderDialog::Init(
	LPCTSTR pszTitle,			// (I) �_�C�A���O�̃^�C�g��
	LPCTSTR pszInitialPath,		// (I) �����t�H���_
	CWnd *pwndParent			// (I) �e�E�B���h�E
)
{
	m_pData = new CFolderDialogData;

	if (pszTitle != NULL)
		m_pData->strTitle = pszTitle;

	if (pszInitialPath != NULL)
		_tcsncpy_s(m_pData->szPath, pszInitialPath, MAX_PATH);

	m_pData->hwndParent = pwndParent ? pwndParent->GetSafeHwnd() : NULL;

}

/*=*************************************************************************//*!
	�_�C�A���O�̕\��
	\return 
*//*=**************************************************************************/
int CFolderDialog::DoModal()
{
	IFileOpenDialog* pFileOpenDialog;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileOpenDialog));
	if (FAILED(hr))
		return 0;

	LPWSTR lpszItem = NULL;
	IShellItem* psiParent = NULL;
	if (PathFileExists(m_pData->szPath)) {
		IShellItem* psiFolder;
		SHCreateItemFromParsingName(m_pData->szPath, NULL, IID_PPV_ARGS(&psiFolder));
		psiFolder->GetParent(&psiParent);

		psiFolder->GetDisplayName(SIGDN_NORMALDISPLAY, &lpszItem);

		pFileOpenDialog->SetFolder(psiParent);
		pFileOpenDialog->SetFileName(lpszItem);
	}

	pFileOpenDialog->SetTitle(m_pData->strTitle);

	DWORD dwOptions;
	pFileOpenDialog->GetOptions(&dwOptions);
	pFileOpenDialog->SetOptions(dwOptions | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);

	int ret = IDCANCEL;

	hr = pFileOpenDialog->Show(m_pData->hwndParent);
	if (SUCCEEDED(hr)) {
		LPWSTR     lpszPath;
		IShellItem *psi;

		hr = pFileOpenDialog->GetResult(&psi);
		if (SUCCEEDED(hr)) {
			psi->GetDisplayName(SIGDN_FILESYSPATH, &lpszPath);
			_tcscpy_s(m_pData->szPath, lpszPath);
			CoTaskMemFree(lpszPath);
			psi->Release();
			ret = IDOK;
		}
	}

	pFileOpenDialog->Release();
	if (psiParent) {
		psiParent->Release();
	}
	CoTaskMemFree(lpszItem);

	return ret;
}

/*=*************************************************************************//*!
	�p�X���̎擾
	\return �p�X������
*//*=**************************************************************************/
CString CFolderDialog::GetPathName() const
{
	return m_pData->szPath;
}

