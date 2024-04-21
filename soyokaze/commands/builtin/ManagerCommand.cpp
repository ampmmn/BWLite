#include "pch.h"
#include "framework.h"
#include "ManagerCommand.h"
#include "commands/core/CommandRepository.h"
#include "icon/IconLoader.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace launcherapp {
namespace commands {
namespace builtin {

CString ManagerCommand::TYPE(_T("Builtin-Manager"));

CString ManagerCommand::GetType()
{
	return TYPE;
}

ManagerCommand::ManagerCommand(LPCTSTR name) :
	BuiltinCommandBase(name ? name : _T("manager"))
{
	mDescription = _T("【キーワードマネージャ】");
}

ManagerCommand::~ManagerCommand()
{
}

BOOL ManagerCommand::Execute(const Parameter& param)
{
	launcherapp::core::CommandRepository::GetInstance()->ManagerDialog();
	return TRUE;
}

HICON ManagerCommand::GetIcon()
{
	return IconLoader::Get()->LoadDefaultIcon();
}

launcherapp::core::Command* ManagerCommand::Clone()
{
	return new ManagerCommand();
}

} // end of namespace builtin
} // end of namespace commands
} // end of namespace launcherapp

