#pragma once

// �L�[���[�h��r�����̂��߂̃C���^�t�F�[�X
class Pattern
{
public:
	virtual ~Pattern() {}

	virtual void SetPattern(const CString& pattern) = 0;
	virtual bool Match(const CString& str) = 0;
};

