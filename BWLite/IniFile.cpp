#include "pch.h"
#include "framework.h"
#include "IniFile.h"

/**
 * @brief  �R���X�g���N�^
 * 
 */
CIniFile::CIniFile()
{
}


/**
 * @brief  �f�X�g���N�^
 * 
 */
CIniFile::~CIniFile()
{
}


/**
 * @brief  INI�t�@�C�����g�p�\�ȏ�Ԃɂ���
 * 
 * @param  pathname �t�@�C���p�X
 */
void CIniFile::Open(LPCTSTR pathname)
{
	ASSERT( pathname != NULL );

	m_pathName = pathname;
}


/**
 * @brief  �t�@�C�������
 */
void CIniFile::Close()
{
	m_pathName.Empty();
}



/**
 * @brief  Open���Ă��邩�H
 * 
 * @return 
 */
bool CIniFile::IsOpen() const
{
	return !m_pathName.IsEmpty();
}


/**
 * @brief  ���ׂẴf�[�^���폜
 */
void CIniFile::RemoveAll()
{
	ASSERT( IsOpen() );

	std::vector<CString> sections;
	GetSectionList(sections);

	for(size_t i=0; i<sections.size(); ++i){
		RemoveSection(sections[i]);
	}
}


/**
 * @brief  �w�肵���Z�N�V�����̃f�[�^����ɂ���
 * 
 * @param  section �Z�N�V������
 */
void CIniFile::RemoveSection(LPCTSTR section)
{
	ASSERT( IsOpen() );

	::WritePrivateProfileString(section, NULL, NULL, m_pathName);
}


/**
 * @brief  �w�肵���L�[���폜����
 * 
 * @param  section �Z�N�V������
 * @param  key     �L�[��
 */
void CIniFile::RemoveKey(LPCTSTR section, LPCTSTR key)
{
	ASSERT( IsOpen() );

	::WritePrivateProfileString(section, key, NULL, m_pathName);
}


/**
 * @brief  �w�肵���L�[�����݂��邩���m�F����
 * 
 * 
 * @param  section �Z�N�V������
 * @param  key     �L�[��
 * 
 * @retval true  ���݂���
 * @retval false ���݂��Ȃ�
 */
bool CIniFile::HasKey(LPCTSTR section, LPCTSTR key) const
{
	CString tmp;
	Read(section, key, tmp, _T(""));
	if( !tmp.IsEmpty() ) return true;

	Read(section, key, tmp, _T("a"));
	if( tmp != _T("a") ) return true;

	return false;
}


/**
 * @brief  �l���o�͂���
 * 
 * @param  section �Z�N�V������
 * @param  key     �L�[��
 * @param  value   �l
 */
void CIniFile::Write(LPCTSTR section, LPCTSTR key, LPCTSTR value)
{
	ASSERT( IsOpen() );

	::WritePrivateProfileString(section, key, value, m_pathName);
}


/**
 * @brief  �l���o�͂���
 * 
 * @param  section �Z�N�V������
 * @param  key     �L�[��
 * @param  value   �l
 */
void CIniFile::Write(LPCTSTR section, LPCTSTR key, int value)
{
	ASSERT( IsOpen() );

	CString str;
	str.Format(_T("%d"), value);
	Write(section, key, str);
}


/**
 * @brief  �l��ǂݍ���
 * 
 * @param  section �Z�N�V������
 * @param  key     �L�[��
 * @param  value   �ǂݍ��݃o�b�t�@
 * @param  defval  �f�t�H���g�l
 */
void CIniFile::Read(LPCTSTR section, LPCTSTR key, CString& value, LPCTSTR defval) const
{
	ASSERT( IsOpen() );

	std::vector<TCHAR> buf;
	for(int i=1; ; ++i){
		buf.resize( 512 * i );
		DWORD res = GetPrivateProfileString(section, key, defval,
				&buf[0], static_cast<DWORD>(buf.size()), m_pathName);

		if( res < (buf.size()-1) ) break;
	}

	value = &buf[0];
}


/**
 * @brief  �l�𐔒l�Ƃ��Ď擾
 * 
 * @param  section �Z�N�V������
 * @param  key     �L�[��
 * @param  defval  �f�t�H���g�l
 * 
 * @return �l
 */
int CIniFile::GetInt(LPCTSTR section, LPCTSTR key, int defval) const
{
	CString value, def;
	def.Format(_T("%d"), defval);
	Read(section, key, value, def);

	return _tcstol(value, NULL, 0);
}


/**
 * @brief  �l�𕶎���Ƃ��Ď擾
 * 
 * @param  section �Z�N�V������
 * @param  key     �L�[��
 * @param  defval  �f�t�H���g�l
 * 
 * @return �l
 */
CString CIniFile::GetString(LPCTSTR section, LPCTSTR key, LPCTSTR defval) const
{
	CString value;
	Read(section, key, value, defval);
	return value;
}


/**
 * @brief  �Z�N�V������񋓂���
 * 
 * @param  sections �Z�N�V�������X�g
 */
void CIniFile::GetSectionList(std::vector<CString>& sections) const
{
	ASSERT( IsOpen() );

	std::vector<TCHAR>	buf;
	for(int i=1; ; ++i){
		buf.resize( 4096 * i );

		DWORD n = GetPrivateProfileString(NULL, NULL, NULL, &buf[0], static_cast<DWORD>(buf.size()), m_pathName);
		if( n < (buf.size() - 2) ) break;
	}


	const TCHAR* p = &buf[0];
	while( *p != _T('\0') ){
		CString key = p;

		sections.push_back(p);

		p += key.GetLength() + 1;
	}
}
