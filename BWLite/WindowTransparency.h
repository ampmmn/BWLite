#pragma once

#include <stdint.h>

class WindowTransparency
{
public:
	WindowTransparency();
	~WindowTransparency();

	void SetWindowHandle(HWND hwnd);

	bool UpdateActiveState(UINT nState);
protected:
	// �ΏۃE�C���h�E�n���h��
	HWND mWindowHandle;

	// ���ߓx(0:���� 255:�s����)
	uint8_t mAlpha;

	// ���ߕ\���@�\�𗘗p���邩?
	bool mIsEnable;

	// ��A�N�e�B�u�̂Ƃ����������ɂ���
	bool mIsInactiveOnly;

};

