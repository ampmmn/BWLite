#pragma once

namespace launcherapp {
namespace mainwindow {
namespace interprocess {

enum EVENT_ID
{
	// $B%3%^%s%IJ8;zNs$rDLCN(B
	SEND_COMMAND = 0,
	// $BA*BrHO0O$rDLCN(B
	SET_CARETRANGE,
	// $B%Q%9$rEPO?(B
	REGISTER_PATH,
	// $B%+%l%s%H%G%#%l%/%H%j$rJQ99(B
	CHANGE_DIRECTORY,
	// $B%&%$%s%I%&$r>C$9(B
	HIDE,
	// $B%&%$%s%I%&$rI=<((B
	SHOW
};

struct SEND_COMMAND_PARAM
{
	bool mIsPasteOnly;
	TCHAR mText[1];
};

struct SET_CARETRANGE_PARAM
{
	int mStartPos;
	int mLength;
};

struct CHANGE_DIRECTORY_PARAM
{
	TCHAR mDirPath[1];
};


}
}
}
