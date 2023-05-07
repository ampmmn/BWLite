#pragma once

struct CFolderDialogData;

// Class: �t�H���_�I���_�C�A���O
class CFolderDialog
{
public:
	CFolderDialog(UINT uTitleID, LPCTSTR pszInitialPath = NULL, CWnd *pwndParent = NULL);
	CFolderDialog(LPCTSTR pszTitle = 0, LPCTSTR pszInitialPath = NULL, CWnd *pwndParent = NULL);
	virtual ~CFolderDialog();

	int DoModal();
	CString GetPathName() const;

protected:
	CFolderDialogData *m_pData;

private:
	void Init(LPCTSTR pszTitle, LPCTSTR pszInitialPath, CWnd *pwndParent);
};
