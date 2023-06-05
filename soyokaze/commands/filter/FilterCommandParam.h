#pragma once


namespace soyokaze {
namespace commands {
namespace filter {

class CommandParam
{
public:
	CommandParam();
	CommandParam(const CommandParam& rhs);
	~CommandParam();

	CommandParam& operator = (const CommandParam& rhs);

public:
	CString mName;
	CString mDescription;
	// 表示方法
	int mShowType;
	// カレントディレクトリ
	CString mDir;
	// パス
	CString mPath;
	// パラメータ
	CString mParameter;
	// 後段のコマンド
	CString mAfterCommandName;
	// 後段のコマンドに渡すパラメータ
	CString mAfterCommandParam;

};

} // end of namespace filter
} // end of namespace commands
} // end of namespace soyokaze

