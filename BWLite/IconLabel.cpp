#include "pch.h"
#include "framework.h"
#include "IconLabel.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ToDo: �A�C�R���\���p�̃R���g���[���쐬

IconLabel::IconLabel()
{
}

IconLabel::~IconLabel()
{
}

BEGIN_MESSAGE_MAP(IconLabel, CStatic)
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void IconLabel::DrawIcon(HICON iconHandle)
{
	CRect rc;
	GetClientRect(rc);

	mMemDC.PatBlt(0,0,rc.Width(), rc.Height(), PATCOPY);

	ASSERT(mMemDC.GetSafeHdc() != NULL);
	mMemDC.DrawIcon(0, 0, iconHandle);

	CClientDC dc(this);
	dc.BitBlt(0,0, rc.Width(), rc.Height(), &mMemDC, 0, 0, SRCCOPY);
}

// �f�t�H���g�A�C�R���̕`��
void IconLabel::DrawDefaultIcon()
{
	if (mIconDefault) {
		DrawIcon(mIconDefault);
	}
}

void IconLabel::OnPaint()
{
	CRect rc;
	GetClientRect(rc);

	CPaintDC dc(this); // device context for painting

	if (mMemDC.GetSafeHdc() == NULL) {
		mMemDC.CreateCompatibleDC(&dc);
		mMemBmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
		mMemDC.SelectObject(mMemBmp);

		COLORREF cr = GetSysColor(COLOR_3DFACE);
		mBkBrush.CreateSolidBrush(cr);

		mMemDC.SelectObject(mBkBrush);
		mMemDC.PatBlt(0,0,rc.Width(), rc.Height(), PATCOPY);

		mIconDefault = AfxGetApp()->LoadIcon(IDI_ICON2);
		mMemDC.DrawIcon(0, 0, mIconDefault);
	}

	dc.BitBlt(0,0, rc.Width(), rc.Height(), &mMemDC, 0, 0, SRCCOPY);
}
