#pragma once

#include <memory>


// あふw(afxw)情報取得用のラッパークラス
class AfxWWrapper
{
public:
	AfxWWrapper();
	~AfxWWrapper();

public:
	// あふの自窓のディレクトリパスを取得
	CString GetCurrentDir();

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;

};

