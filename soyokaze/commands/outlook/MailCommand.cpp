#include "pch.h"
#include "framework.h"
#include "MailCommand.h"
#include "commands/outlook/OutlookItems.h"
#include "IconLoader.h"
#include "SharedHwnd.h"
#include "resource.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace soyokaze {
namespace commands {
namespace outlook {


struct MailCommand::PImpl
{
	MailItem* mMailItem;
};

MailCommand::MailCommand(
	MailItem* itemPtr
) : in(std::make_unique<PImpl>())
{
	in->mMailItem = itemPtr;
	itemPtr->AddRef();

	this->mName = itemPtr->GetSubject();
	this->mDescription = itemPtr->GetSubject();
}


MailCommand::~MailCommand()
{
	if (in->mMailItem) {
		in->mMailItem->Release();
	}
}

CString MailCommand::GetTypeDisplayName()
{
	return _T("Outlookメール");
}

BOOL MailCommand::Execute(const Parameter& param)
{
	// Ctrlキーが押されていたら最大化表示する
	bool isShowMaximize = param.GetNamedParamBool(_T("CtrlKeyPressed"));

	if (in->mMailItem) {
		return in->mMailItem->Activate(isShowMaximize);
	}
	return FALSE;
}

HICON MailCommand::GetIcon()
{
	return IconLoader::Get()->LoadUnknownIcon();
	// ToDo: 実装
	//// 拡張子に関連付けられたアイコンを取得
	//LPCTSTR fileExt = PathFindExtension(in->mCalcWorksheet->GetWorkbookName());
	//if (_tcslen(fileExt) == 0) {
	//	return IconLoader::Get()->LoadUnknownIcon();
	//}
	//return IconLoader::Get()->LoadExtensionIcon(fileExt);
}

soyokaze::core::Command*
MailCommand::Clone()
{
	return new MailCommand(in->mMailItem);
}

} // end of namespace outlook
} // end of namespace commands
} // end of namespace soyokaze
