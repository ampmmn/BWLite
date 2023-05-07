#include "pch.h"
#include "framework.h"
#include "AppProfile.h"
#include "IniFile.h"

#include <shlwapi.h>
#include <atlpath.h>
#include <wincrypt.h>


#pragma comment(lib,"crypt32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//! �ݒ���쐬�p�̃t�H���_������
bool CAppProfile::CreateProfileDirectory()
{
	TCHAR path[1024];
	GetDirPath(path, 1024);
	if (PathIsDirectory(path)) {
		return true;
	}
	// �t�H���_�Ȃ���΂���
	CString msg;
	msg.Format(_T("�y����N���z\n�ݒ�t�@�C���� %s �ȉ��ɍ쐬����܂��B"), path);
	AfxMessageBox(msg);

	return CreateDirectory(path, NULL) != FALSE;
}

// �f�B���N�g���p�X���擾
const TCHAR* CAppProfile::GetDirPath(TCHAR* path, size_t len)
{
	size_t buflen = 1024;
	TCHAR buff[1024];
	_tgetenv_s(&buflen, buff, _T("USERPROFILE"));

	_tcscpy_s(path, len, buff);

	PathAppend(path, _T(".bwlite"));

	return path;
}

// �t�@�C���p�X���擾
const TCHAR* CAppProfile::GetFilePath(TCHAR* path, size_t len)
{
	GetDirPath(path, len);
	PathAppend(path, _T("BWLite.ini"));

	return path;
}

/*!
 *	@brief �f�t�H���g�R���X�g���N�^
*/
 CAppProfile::CAppProfile() : m_entity(new CIniFile())
{
	TCHAR path[1024];
	GetFilePath(path, 1024);
	m_entity->Open((LPCTSTR)path);
}

/*!
 *	@brief �f�X�g���N�^
*/
 CAppProfile::~CAppProfile()
{
	delete m_entity;
}

/*!
 *	@brief �C���X�^���X�̐����E�擾
 *	@return �������ꂽ�C���X�^���X�ւ̃|�C���^
*/
CAppProfile* CAppProfile::Get()
{
	static CAppProfile s_inst;
	return &s_inst;
}

/*!
 *	@brief �����l���擾���܂��B
 *	@return �擾���������l
 *	@param[in] section �Z�N�V������
 *	@param[in] key   �L�[��
 *	@param[in] def     �Ή�����l�����݂��Ȃ��ꍇ�ɕԂ��l
*/
int CAppProfile::Get(
	LPCTSTR section,
	LPCTSTR key,
	int def
)
{
	ASSERT(m_entity);
	return m_entity->GetInt(section, key, def);
}

/*!
 *	@brief �����l��ݒ�
 *	@return this�I�u�W�F�N�g�ւ̎Q�Ƃ�Ԃ��܂�
 *	@param[in] section �Z�N�V������
 *	@param[in] key     �L�[��
 *	@param[in] value   �o�͂���l
*/
CAppProfile& CAppProfile::Write(
	LPCTSTR section,
	LPCTSTR key,
	int value
)
{
	ASSERT(m_entity);
	m_entity->Write(section, key, value);
	return *this;
}


/*!
 *	@brief �����l���擾���܂��B
 *	@return �擾���������l
 *	@param[in] section �Z�N�V������
 *	@param[in] key     �L�[��
 *	@param[in] def     �Ή�����l�����݂��Ȃ��ꍇ�ɕԂ��l
*/
double CAppProfile::Get(
	LPCTSTR section,
	LPCTSTR key,
	double def
)
{
	ASSERT(m_entity);

	CString str;
	str.Format(_T("%g"), def);

	CString retVal = m_entity->GetString(section, key, str);
	
	double d;
	if (_stscanf_s(retVal, _T("%lf"), &d) != 0) {
		return def;
	}
	return d;
}

/*!
 *	@brief �����l��ݒ�
 *	@return this�I�u�W�F�N�g�ւ̎Q�Ƃ�Ԃ��܂�
 *	@param[in] section �Z�N�V������
 *	@param[in] key     �L�[��
 *	@param[in] value     �o�͂���l
*/
CAppProfile& CAppProfile::Write(
	LPCTSTR section,
	LPCTSTR key,
	double value
)
{
	ASSERT(m_entity);

	CString str;
	str.Format(_T("%f"), value);

	m_entity->Write(section, key, str);
	return *this;
}


/*!
 *	@brief ������l���擾���܂��B
 *	@return �擾����������l
 *	@param[in] section �Z�N�V������
 *	@param[in] key     �L�[��
 *	@param[in] def     �Ή�����l�����݂��Ȃ��ꍇ�ɕԂ��l
*/
CString CAppProfile::Get(
	LPCTSTR section,
	LPCTSTR key,
	LPCTSTR def
)
{
	return GetString(section, key, def);
}

CString CAppProfile::GetString(
	LPCTSTR section,
	LPCTSTR key,
	LPCTSTR def
)
{
	ASSERT(m_entity);
	return m_entity->GetString(section, key, def);
}

/*!
 *	@brief �������ݒ肵�܂��B
 *	@return this�I�u�W�F�N�g�ւ̎Q�Ƃ�Ԃ��܂�
 *	@param[in] section �Z�N�V������
 *	@param[in] key     �L�[��
 *	@param[in] value   �o�͂���l
*/
CAppProfile& CAppProfile::Write(
	LPCTSTR section,
	LPCTSTR key,
	LPCTSTR value
)
{
	return WriteString(section, key, value);
}

CAppProfile& CAppProfile::WriteString(
	LPCTSTR section,
	LPCTSTR key,
	LPCTSTR value
)
{
	ASSERT(m_entity);
	m_entity->Write(section, key, value);
	return *this;
}


//! �o�C�g����擾
size_t CAppProfile::Get(LPCTSTR section, LPCTSTR key, void* out, size_t len)
{
	return GetBinary(section, key, out, len);
}

//! �o�C�g����擾
size_t CAppProfile::GetBinary(LPCTSTR section, LPCTSTR key, void* out, size_t len)
{
	CString str = Get(section, key, _T(""));
	if (str.IsEmpty()) {
		return 0;
	}

	DWORD nBinLen;
	DWORD dwSkip, dwFlags;
	if (CryptStringToBinary(str, str.GetLength(), CRYPT_STRING_BASE64, NULL, &nBinLen, &dwSkip, &dwFlags) == FALSE) {
		return 0;
	}

	if (out == NULL) {
		return nBinLen;
	}
	if (len < nBinLen) {
		return nBinLen;
	}

	VERIFY(CryptStringToBinary(str, str.GetLength(), CRYPT_STRING_BASE64, (LPBYTE)out, &nBinLen, &dwSkip, &dwFlags));
	return nBinLen;
}

//�I �o�C�g���ݒ�
CAppProfile& CAppProfile::Write(LPCTSTR section, LPCTSTR key, const void* data, size_t len)
{
	return WriteBinary(section, key, data, len);
}

//�I �o�C�g���ݒ�
CAppProfile& CAppProfile::WriteBinary(LPCTSTR section, LPCTSTR key, const void* data, size_t len)
{
	DWORD nStrLen = 0;
	CryptBinaryToString((const LPBYTE)data, (DWORD)len, CRYPT_STRING_BASE64, NULL, &nStrLen);

	CString str;
	CryptBinaryToString((const LPBYTE)data, (DWORD)len, CRYPT_STRING_BASE64, str.GetBuffer(nStrLen), &nStrLen);
	str.ReleaseBuffer();
	return Write(section, key, (LPCTSTR)str);
}

/*!
 *	@brief ��������o�C�g��Ƃ��ĕۑ�
 *	@return 
 *	@param[in] section 
 *	@param[in] key     
 *	@param[in] value   
 */
CAppProfile& CAppProfile::WriteStringAsBinary(
	LPCTSTR section,
	LPCTSTR key,
	LPCTSTR value
)
{
	size_t n_strings = _tcslen(value) + 1;   // + 1 NUL�I�[����
	size_t n_bytes = n_strings * sizeof(TCHAR);
	return WriteBinary(section, key, value, n_bytes);
}

/*!
 *	@brief �o�C�g��𕶎���Ƃ��Ď擾
 *	@return 
 *	@param[in] section 
 *	@param[in] key     
 *	@param[in] def     
 */
CString CAppProfile::GetBinaryAsString(
	LPCTSTR section,
	LPCTSTR key,
	LPCTSTR def
)
{
	size_t len_in_byte = GetBinary(section, key, NULL, 0);
	if (len_in_byte == 0) {
		return def;
	}

	size_t len_in_char = len_in_byte / sizeof(TCHAR);

	CString str;
	GetBinary(section, key, str.GetBuffer((int)len_in_char), len_in_byte);
	str.ReleaseBuffer();

	return str;
}


