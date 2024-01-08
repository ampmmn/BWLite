#pragma once

namespace soyokaze {
namespace commands {
namespace simple_dict {

class SimpleDictParam
{
public:
	CString mName;
	CString mDescription;

	// $B%U%!%$%k$N%Q%9(B
	CString mFilePath;
	// $B%7!<%HL>(B
	CString mSheetName;
	// $BHO0O(B
	CString mRange;
	// $B@hF,9T$O%X%C%@$+(B?
	BOOL mIsFirstRowHeader; 
};

}
}
}

