#pragma once

// �R�}���h���s����
class ExecHistory
{
public:
	ExecHistory();
	~ExecHistory();

public:
	// ���s�����R�}���h�������ǉ�
	void Add(const CString& commandStr);

	// �w�肵���R�}���h�����񂪑O����s���ꂽ����\�����l���擾����
	size_t GetOrder(const CString& commandStr);

	// 
	void SetLimit(int limit);

	void Load();
	void Save();

protected:
	struct PImpl;
	PImpl* in;
};

