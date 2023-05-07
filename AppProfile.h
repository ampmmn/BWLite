#pragma once

class CIniFile;

// �A�v���P�[�V�����̃v���t�@�C�����ɃA�N�Z�X���邽�߂̃N���X
// �V���O���g���N���X
class CAppProfile
{
	CAppProfile();
	~CAppProfile();
	
	// �R�s�[�֎~
	CAppProfile(const CAppProfile&);
	CAppProfile& operator = (const CAppProfile& );
public:
	//! �C���X�^���X�̐����E�擾
	static CAppProfile* Get();

	//! �����l���擾
	int Get(LPCTSTR section, LPCTSTR key, int def);
	//! �����l��ݒ�
	CAppProfile& Write(LPCTSTR section, LPCTSTR key, int value);

	//! �����l���擾
	double Get(LPCTSTR section, LPCTSTR key, double def);
	//! �����l��ݒ�
	CAppProfile& Write(LPCTSTR section, LPCTSTR key, double value);

	//! ��������擾
	CString Get(LPCTSTR section, LPCTSTR key, LPCTSTR def = _T(""));
	CString GetString(LPCTSTR section, LPCTSTR key, LPCTSTR def = _T(""));
	//! �������ݒ�
	CAppProfile& Write(LPCTSTR section, LPCTSTR key, LPCTSTR value);
	CAppProfile& WriteString(LPCTSTR section, LPCTSTR key, LPCTSTR value);

	//! �o�C�g����擾
	size_t Get(LPCTSTR section, LPCTSTR key, void* out, size_t len);
	size_t GetBinary(LPCTSTR section, LPCTSTR key, void* out, size_t len);

	//! �o�C�g����擾
	template <typename CONTAINER>
	size_t Get(LPCTSTR section, LPCTSTR key, CONTAINER& out) {
		size_t n = Get(section, key, NULL, 0);
		if (n == 0) {
			return 0;
		}
		size_t count = n / sizeof(typename CONTAINER::value_type);
		out.resize(out);
		return Get(section, key, (void*)&out.front(), n);
	}
	//�I �o�C�g���ݒ�
	CAppProfile& Write(LPCTSTR section, LPCTSTR key, const void* data, size_t len);
	CAppProfile& WriteBinary(LPCTSTR section, LPCTSTR key, const void* data, size_t len);
	//! �o�C�g���ݒ�
	template <typename CONTAINER>
	CAppProfile& Write(LPCTSTR section, LPCTSTR key, const CONTAINER& data) {
		size_t n_bytes = data.size() * sizeof(typename CONTAINER::value_type);
		return Write(section, key, &data.front(), n_bytes);
	}

	//! ��������o�C�g��Ƃ��ĕۑ�
	CAppProfile& WriteStringAsBinary(LPCTSTR section, LPCTSTR key, LPCTSTR value);
	//! �o�C�g��𕶎���Ƃ��Ď擾
	CString GetBinaryAsString(LPCTSTR section, LPCTSTR key, LPCTSTR def = _T(""));

private:
	CIniFile* m_entity;  //!< ���ۂ̕ۑ���I�u�W�F�N�g
};

