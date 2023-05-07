#pragma once

#include <vector>

/**
 * @brief  INI�t�H�[�}�b�g�t�@�C���N���X
 * 
 * Windows��Ini�t�@�C���`���̓��o�͂��s���܂�
 */
class CIniFile
{
public:
	CIniFile();
	~CIniFile();

public:
	//! 
	void Open(LPCTSTR pathname);
	//! 
	void Close();
	//! 
	bool IsOpen() const;

	//! ���ׂẴf�[�^���폜
	void RemoveAll();
	//! �w�肵���Z�N�V��������ɂ���
	void RemoveSection(LPCTSTR section);
	//! �w�肵���L�[���폜����
	void RemoveKey(LPCTSTR section, LPCTSTR key);

	//! �L�[�����݂��邩�H
	bool HasKey(LPCTSTR section, LPCTSTR key) const;

	//! �l�̏o��
	void Write(LPCTSTR section, LPCTSTR key, LPCTSTR value);
	//! �l�̏o��
	void Write(LPCTSTR section, LPCTSTR key, int value);
	//! �l�̓���
	void Read(LPCTSTR section, LPCTSTR key, CString& value, LPCTSTR defval) const;

	//! �Z�N�V�����̃��X�g���擾
	void GetSectionList(std::vector<CString>& sections) const;

	//! �l�̓���
	int     GetInt(LPCTSTR section, LPCTSTR key, int defval=0) const;
	//! �l�̓���
	CString GetString(LPCTSTR section, LPCTSTR key, LPCTSTR defval=_T("")) const;

public:
	CString	m_pathName;	//!< Ini�t�@�C���p�X
};
