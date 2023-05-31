#include "pch.h"
#include "NamedCommandHotKeyHandler.h"
#include "core/CommandRepository.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NamedCommandHotKeyHandler::NamedCommandHotKeyHandler(
	CString name
) : mName(name)
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
	auto cmdRepoPtr = soyokaze::core::CommandRepository::GetInstance();
	auto cmd = cmdRepoPtr->QueryAsWholeMatch(mName, false);
	if (cmd == nullptr) {
		return false;
	}

	// $BF~NOMs$rHsI=<($K$7$F!"%3%^%s%I$r<B9T$9$k!#(B
	ShowWindow(AfxGetMainWnd()->GetSafeHwnd(), SW_HIDE);

	bool result = cmd->Execute();
	cmd->Release();

	return result;
}

