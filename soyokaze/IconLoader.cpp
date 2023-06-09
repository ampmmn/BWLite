#include "pch.h"
#include "framework.h"
#include "IconLoader.h"
#include "resource.h"
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct IconLoader::PImpl
{
	PImpl() : 
		mEditIcon(nullptr),
		mKeywordManagerIcon(nullptr)
	{
		const LPCTSTR SYSTEMROOT = _T("SystemRoot");

		size_t reqLen = 0;
		_tgetenv_s(&reqLen, mImgResDll, MAX_PATH_NTFS, SYSTEMROOT);
		PathAppend(mImgResDll, _T("System32"));
		PathAppend(mImgResDll, _T("imageres.dll"));

		_tgetenv_s(&reqLen, mShell32Dll, MAX_PATH_NTFS, SYSTEMROOT);
		PathAppend(mShell32Dll, _T("System32"));
		PathAppend(mShell32Dll, _T("shell32.dll"));

	}

	HICON GetImageResIcon(int index)
	{
		HICON icon[1];
		UINT n = ExtractIconEx(mImgResDll, index, icon, NULL, 1);
		return (n == 1) ? icon[0]: NULL;
	}
	HICON GetShell32Icon(int index)
	{
		HICON icon[1];
		UINT n = ExtractIconEx(mShell32Dll, index, icon, NULL, 1);
		return (n == 1) ? icon[0]: NULL;
	}


	TCHAR mImgResDll[MAX_PATH_NTFS];
	TCHAR mShell32Dll[MAX_PATH_NTFS];
	std::map<int, HICON> mShell32IconCache;
	std::map<int, HICON> mImageResIconCache;
	HICON mEditIcon;
	HICON mKeywordManagerIcon;
};

IconLoader::IconLoader() : in(new PImpl)
{
}

IconLoader::~IconLoader()
{
	delete in;
}

IconLoader* IconLoader::Get()
{
	static IconLoader instance;
	return &instance;
}

HICON IconLoader::LoadIconFromPath(const CString& path)
{
	if (PathIsDirectory(path)) {
		return LoadFolderIcon();
	}
	if (PathFileExists(path)) {
		SHFILEINFO sfi = {};
		HIMAGELIST hImgList =
			(HIMAGELIST)::SHGetFileInfo(path, 0, &sfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_LARGEICON | SHGFI_USEFILEATTRIBUTES);
		HICON hIcon = sfi.hIcon;
		return hIcon;
	}
	if (path.Find(_T("http")) == 0) {
		return LoadWebIcon();
	}
	return LoadUnknownIcon();
}

HICON IconLoader::GetShell32Icon(int index)
{
	auto it = in->mShell32IconCache.find(index);
	if (it != in->mShell32IconCache.end()) {
		return it->second;
	}

	HICON h =in->GetShell32Icon(index);
	if (h == NULL) {
		return nullptr;
	}

	in->mShell32IconCache[index] = h;
	return h;
}

HICON IconLoader::GetImageResIcon(int index)
{
	auto it = in->mImageResIconCache.find(index);
	if (it != in->mImageResIconCache.end()) {
		return it->second;
	}

	HICON h =in->GetImageResIcon(index);
	if (h == NULL) {
		return nullptr;
	}

	in->mImageResIconCache[index] = h;
	return h;
}


HICON IconLoader::LoadFolderIcon()
{
	return GetImageResIcon(3);
}

HICON IconLoader::LoadWebIcon()
{
	return GetImageResIcon(20);
}

HICON IconLoader::LoadNewIcon()
{
	// ToDo: 実装
	return AfxGetApp()->LoadIcon(IDI_ICON2);
}

HICON IconLoader::LoadSettingIcon()
{
	return GetImageResIcon(109);
}

HICON IconLoader::LoadExitIcon()
{
	return GetImageResIcon(235);
}

HICON IconLoader::LoadEditIcon()
{
	// ToDo: 実装
	return in->mEditIcon;
}

HICON IconLoader::LoadKeywordManagerIcon()
{
	// ToDo: 実装
	return in->mKeywordManagerIcon;
}

HICON IconLoader::LoadDefaultIcon()
{
	return AfxGetApp()->LoadIcon(IDI_ICON2);
}

HICON IconLoader::LoadUserDirIcon()
{
	return GetImageResIcon(157);
}

HICON IconLoader::LoadMainDirIcon()
{
	return GetImageResIcon(157);
}

HICON IconLoader::LoadVersionIcon()
{
	return AfxGetApp()->LoadIcon(IDI_ICON2);
}

HICON IconLoader::LoadTasktrayIcon()
{
	return AfxGetApp()->LoadIcon(IDI_ICON2);
}

HICON IconLoader::LoadUnknownIcon()
{
	return GetImageResIcon(2);
}

HICON IconLoader::LoadReloadIcon()
{
	return GetImageResIcon(228);
}

HICON IconLoader::LoadRegisterWindowIcon()
{
	return GetImageResIcon(19);
}

HICON IconLoader::LoadGroupIcon()
{
	return GetShell32Icon(250);
}

