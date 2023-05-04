#include "pch.h"
#include "framework.h"
#include "ReloadCommand.h"
#include "CommandMap.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ReloadCommand::ReloadCommand(CommandMap* pMap) :
	m_pCommandMap(pMap)
{
}

ReloadCommand::~ReloadCommand()
{
}

CString ReloadCommand::GetName()
{
	return _T("reload");
}

CString ReloadCommand::GetDescription()
{
	return _T("設定のリロード");
}

BOOL ReloadCommand::Execute()
{
	return m_pCommandMap->Load();
}

BOOL ReloadCommand::Execute(const std::vector<CString>& args)
{
	// 引数指定しても動作はかわらない
	return Execute();
}

CString ReloadCommand::GetErrorString()
{
	return _T("");
}

HICON ReloadCommand::GetIcon()
{
	return AfxGetApp()->LoadIcon(IDI_ICON2);
}

BOOL ReloadCommand::Match(Pattern* pattern)
{
	return pattern->Match(GetName());
}

