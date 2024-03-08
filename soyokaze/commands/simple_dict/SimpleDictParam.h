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
	CString mRangeFront;
	CString mRangeBack;
	// $B@hF,9T$O%X%C%@$+(B?
	BOOL mIsFirstRowHeader; 
	// $B%3%^%s%IL>$rF~NO$7$J$/$F$b8uJd$rI=<($9$k(B
	BOOL mIsMatchWithoutKeyword;
	// $B5U0z$-$rM-8z$K$9$k(B
	BOOL mIsEnableReverse;
	// $B8eCJ$N=hM}$N<oN`(B
	int mActionType;
	// $B8eCJ$N%3%^%s%I(B(mPostFilterType=0$B$N>l9g(B)
	CString mAfterCommandName;
	// $B8eCJ$N%U%!%$%k(BorURL(mPostFilterType=1$B$N>l9g(B)
	CString mAfterFilePath;
	// $B8eCJ$N%3%^%s%I$KEO$9%Q%i%a!<%?(B
	CString mAfterCommandParam;
};

}
}
}

