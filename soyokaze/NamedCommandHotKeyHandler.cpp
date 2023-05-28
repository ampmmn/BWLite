#include "pch.h"
#include "NamedCommandHotKeyHandler.h"
#include "CommandRepository.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NamedCommandHotKeyHandler::NamedCommandHotKeyHandler(
	CommandRepository* cmdReposPtr,
	CString name
) : mCmdReposPtr(cmdReposPtr), mName(name)
{
}

NamedCommandHotKeyHandler::~NamedCommandHotKeyHandler()
{
}


CString NamedCommandHotKeyHandler::GetDisplayName()
{
	return mName;
}

bool NamedCommandHotKeyHandler::Invoke()
{
	auto cmd = mCmdReposPtr->QueryAsWholeMatch(mName, false);
	if (cmd == nullptr) {
		return false;
	}

	// $BF~NOMs$rHsI=<($K$7$F!"%3%^%s%I$r<B9T$9$k!#(B
	ShowWindow(AfxGetMainWnd()->GetSafeHwnd(), SW_HIDE);

	return cmd->Execute();
}

