#include "pch.h"
#include "GroupEditDialog.h"
#include "gui/CommandHotKeyDialog.h"
#include "gui/ModalComboBox.h"
#include "core/CommandRepository.h"
#include "utility/Accessibility.h"
#include "resource.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using Command =  soyokaze::core::Command;

namespace soyokaze {
namespace commands {
namespace group {



GroupEditDialog::GroupEditDialog() : 
	CDialogEx(IDD_GROUP),
	mCommandListPtr(nullptr),
	mCommandSelectBox(std::make_unique<ModalComboBox>())
{
}

GroupEditDialog::~GroupEditDialog()
{
}

void GroupEditDialog::SetParam(const CommandParam& param)
{
	mOrgName = param.mName;
	mParam = param;
}

const CommandParam& GroupEditDialog::GetParam() const
{
	return mParam;
}

void GroupEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_STATUSMSG, mMessage);
	DDX_Text(pDX, IDC_EDIT_NAME, mParam.mName);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, mParam.mDescription);
	DDX_Check(pDX, IDC_CHECK_PARAM, mParam.mIsPassParam);
	DDX_Check(pDX, IDC_CHECK_REPEAT, mParam.mIsRepeat);
	DDX_Text(pDX, IDC_EDIT_REPEATS, mParam.mRepeats);
	DDV_MinMaxInt(pDX, mParam.mRepeats, 1, 0x7fffffff);
	DDX_Check(pDX, IDC_CHECK_CONFIRM, mParam.mIsConfirm);
	DDX_Text(pDX, IDC_EDIT_HOTKEY2, mHotKey);
}

BEGIN_MESSAGE_MAP(GroupEditDialog, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnUpdate)
	ON_COMMAND(IDC_CHECK_REPEAT, OnUpdate)
	ON_COMMAND(IDC_BUTTON_HOTKEY, OnButtonHotKey)
	ON_WM_CTLCOLOR()
	ON_COMMAND(IDC_BUTTON_ADD, OnButtonAdd)
	ON_COMMAND(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_COMMAND(IDC_BUTTON_UP, OnButtonUp)
	ON_COMMAND(IDC_BUTTON_DOWN, OnButtonDown)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COMMANDS, OnNotifyItemChanged)
	ON_NOTIFY(NM_CLICK, IDC_LIST_COMMANDS, OnNotifyItemClick)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COMMANDS, OnNotifyItemDblClk)

END_MESSAGE_MAP()


BOOL GroupEditDialog::OnInitDialog()
{
	__super::OnInitDialog();

	mCommandListPtr = (CListCtrl*)GetDlgItem(IDC_LIST_COMMANDS);
	ASSERT(mCommandListPtr);

	mCommandListPtr->SetExtendedStyle(mCommandListPtr->GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	// コマンド一覧コンボボックスを作っておく
	mCommandSelectBox->Create(mCommandListPtr, WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL | CBS_NOINTEGRALHEIGHT);
	mCommandSelectBox->SetMinVisibleItems(10);
	std::vector<Command*> commands;
	soyokaze::core::CommandRepository::GetInstance()->EnumCommands(commands);
	for (auto& cmd : commands) {
		mCommandSelectBox->AddString(cmd->GetName());
	}

	CString caption;
  GetWindowText(caption);

	CString suffix;
	suffix.Format(_T("【%s】"), mOrgName.IsEmpty() ? _T("新規作成") : (LPCTSTR)mOrgName);

	caption += suffix;
	SetWindowText(caption);

	// ヘッダー追加
	ASSERT(mCommandListPtr);

	LVCOLUMN lvc;
	memset(&lvc,0,sizeof(LV_COLUMN));
	lvc.mask = LVCF_TEXT|LVCF_FMT|LVCF_WIDTH;

	CString strHeader;
	strHeader.LoadString(IDS_NAME);
	lvc.pszText = const_cast<LPTSTR>((LPCTSTR)strHeader);
	lvc.cx = 200;
	lvc.fmt = LVCFMT_LEFT;
	mCommandListPtr->InsertColumn(0,&lvc);

	strHeader.LoadString(IDS_ISWAIT);
	lvc.pszText = const_cast<LPTSTR>((LPCTSTR)strHeader);
	lvc.cx = 150;
	lvc.fmt = LVCFMT_LEFT;
	mCommandListPtr->InsertColumn(1,&lvc);

	// 項目を登録
	int index = 0;
	for (auto& item : mParam.mItems) {

		if (mOrgName == item.mItemName) {
			// 自分自身は追加しない
			continue;
		}

		SetItemToList(index++, item);
	}

	UpdateStatus();
	UpdateData(FALSE);

	return TRUE;
}

bool GroupEditDialog::UpdateStatus()
{
	GetDlgItem(IDC_EDIT_REPEATS)->EnableWindow(mParam.mIsRepeat);

	// 32を上限とする
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(mParam.mItems.size() < 32);

	mHotKey = mHotKeyAttr.ToString();
	if (mHotKey.IsEmpty()) {
		mHotKey.LoadString(IDS_NOHOTKEY);
	}

	BOOL canCreate = TRUE;

	if (mParam.mName.IsEmpty()) {
		mMessage.LoadString(IDS_ERR_NAMEISEMPTY);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		canCreate = FALSE;
	}
	if (canCreate && mParam.mItems.empty()) {
		mMessage.LoadString(IDS_ERR_GROUPITEMEMPTY);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		canCreate = FALSE;
	}

	// ボタンの状態を更新
	POSITION pos = mCommandListPtr->GetFirstSelectedItemPosition();
	bool hasSelect = pos != NULL;

	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(hasSelect);
	if (hasSelect) {
		int itemIndex = mCommandListPtr->GetNextSelectedItem(pos);
		GetDlgItem(IDC_BUTTON_UP)->EnableWindow(itemIndex > 0);
		GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(itemIndex < mCommandListPtr->GetItemCount()-1);
	}
	else {
		GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(FALSE);
	}


	auto cmdRepoPtr = soyokaze::core::CommandRepository::GetInstance();

	// ToDo: 存在しないコマンドがあったら警告

	// 重複チェック
	if (mParam.mName.CompareNoCase(mOrgName) != 0) {
		auto cmd = cmdRepoPtr->QueryAsWholeMatch(mParam.mName, false);
		if (cmd != nullptr) {
			cmd->Release();
			mMessage.LoadString(IDS_ERR_NAMEALREADYEXISTS);
			GetDlgItem(IDOK)->EnableWindow(FALSE);
		}
	}

	// 使えない文字チェック
	if (cmdRepoPtr->IsValidAsName(mParam.mName) == false) {
		mMessage.LoadString(IDS_ERR_ILLEGALCHARCONTAINS);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		return false;
	}

	if (canCreate) {
		mMessage.Empty();
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}

	return true;
}

void GroupEditDialog::SwapItem(int srcIndex, int dstIndex)
{
	for (int col = 0; col < 2; ++col) {
		CString srcText = mCommandListPtr->GetItemText(srcIndex, col);
		CString dstText = mCommandListPtr->GetItemText(dstIndex, col);

		mCommandListPtr->SetItemText(srcIndex, col, dstText);
		mCommandListPtr->SetItemText(dstIndex, col, srcText);
	}

	// 選択
	mCommandListPtr->SetItemState(dstIndex, 0, LVIS_SELECTED);
	mCommandListPtr->SetItemState(dstIndex, LVIS_SELECTED, LVIS_SELECTED);

	std::swap(mParam.mItems[srcIndex],mParam.mItems[dstIndex]);
}

void GroupEditDialog::SetItemToList(int index, const GroupItem& item)
{
	ASSERT(mCommandListPtr);
	if (index < 0) {
		return ;
	}

	if (index == mCommandListPtr->GetItemCount()) {
		mCommandListPtr->InsertItem(index, item.mItemName);
	}
	else {
		mCommandListPtr->SetItemText(index, 0, item.mItemName);
	}


	CString checked =item.mIsWait ? _T("\U00002714") : _T("");
	mCommandListPtr->SetItemText(index, 1, checked);
}


void GroupEditDialog::OnUpdate()
{
	UpdateData();
	UpdateStatus();
	UpdateData(FALSE);
}

HBRUSH GroupEditDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (utility::IsHighContrastMode()) {
		return __super::OnCtlColor(pDC, pWnd, nCtlColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_STATUSMSG) {
		COLORREF crTxt = mMessage.IsEmpty() ? RGB(0,0,0) : RGB(255, 0, 0);
		pDC->SetTextColor(crTxt);
	}
	return __super::OnCtlColor(pDC, pWnd, nCtlColor);
}

void GroupEditDialog::OnOK()
{
	UpdateData();
	if (UpdateStatus() == false) {
		return ;
	}

	__super::OnOK();
}


void GroupEditDialog::OnButtonHotKey()
{
	UpdateData();

	CommandHotKeyDialog dlg(mHotKeyAttr);
	dlg.mIsGlobal = mIsGlobal;
	if (dlg.DoModal() != IDOK) {
		return ;
	}

	dlg.GetAttribute(mHotKeyAttr);
	mIsGlobal = dlg.IsGlobal();

	UpdateStatus();
	UpdateData(FALSE);
}

bool GroupEditDialog::SelectCommand(int index)
{
	// コンボボックスを表示
	ModalComboBox* cmbBox = mCommandSelectBox.get();
	if (cmbBox->DoModalOverListItem(mCommandListPtr, index) != IDOK) {
		return false;
	}

	if (cmbBox->GetCurSel() == -1) {
		return false;
	}

	// 選択したテキストでセルを更新
	CString cmdName;
	cmbBox->GetLBText(cmbBox->GetCurSel(), cmdName);

	// 

	CString checked;

	ASSERT(index <= mParam.mItems.size());
	if (index == mParam.mItems.size()) {
		GroupItem item;
		item.mItemName = cmdName;
		item.mIsWait = true;
		mParam.mItems.push_back(item);
	}
	else {
		mParam.mItems[index].mItemName = cmdName;
	}
	SetItemToList(index, mParam.mItems[index]);
	return true;
}


void GroupEditDialog::OnButtonAdd()
{
	ASSERT(mCommandListPtr);

	int nItemCount = mCommandListPtr->GetItemCount();

	CRect rc;
	mCommandListPtr->GetItemRect(0, &rc, LVIR_BOUNDS);
	int offset = rc.Height() * (nItemCount - mCommandListPtr->GetTopIndex());
	CSize cs;
	cs.cx = 0;
	cs.cy = offset;
	if (offset) {
		mCommandListPtr->Scroll(cs);
	}

	int index = mCommandListPtr->InsertItem(nItemCount, _T(""));

	if (SelectCommand(index) == false) {
		mCommandListPtr->DeleteItem(nItemCount);
		return ;
	}
}

void GroupEditDialog::OnButtonDelete()
{
	POSITION pos = mCommandListPtr->GetFirstSelectedItemPosition();
	if (pos == NULL) {
		return;
	}

	int itemIndex = mCommandListPtr->GetNextSelectedItem(pos);
	mCommandListPtr->DeleteItem(itemIndex);
	mParam.mItems.erase(mParam.mItems.begin() + itemIndex);

	if (itemIndex < mCommandListPtr->GetItemCount()) {
		mCommandListPtr->SetItemState(itemIndex, LVIS_SELECTED, LVIS_SELECTED);
	}
	else {
		itemIndex--;
		if (0 <= itemIndex && itemIndex < mCommandListPtr->GetItemCount()) {
			mCommandListPtr->SetItemState(itemIndex, LVIS_SELECTED, LVIS_SELECTED);
		}
	}

	UpdateStatus();
	UpdateData(FALSE);
}

void GroupEditDialog::OnButtonUp()
{
	POSITION pos = mCommandListPtr->GetFirstSelectedItemPosition();
	if (pos == NULL) {
		return;
	}

	int itemIndex = mCommandListPtr->GetNextSelectedItem(pos);
	if (itemIndex == 0) {
		return;
	}
	SwapItem(itemIndex, itemIndex-1);

	UpdateStatus();
	UpdateData(FALSE);
}

void GroupEditDialog::OnButtonDown()
{
	POSITION pos = mCommandListPtr->GetFirstSelectedItemPosition();
	if (pos == NULL) {
		return;
	}

	int itemIndex = mCommandListPtr->GetNextSelectedItem(pos);
	if (itemIndex == mCommandListPtr->GetItemCount()-1) {
		return;
	}
	SwapItem(itemIndex, itemIndex+1);

	UpdateStatus();
	UpdateData(FALSE);
}

void GroupEditDialog::OnNotifyItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateStatus();
	UpdateData(FALSE);
	*pResult = 0;
}

void GroupEditDialog::OnNotifyItemClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	ASSERT(mCommandListPtr);

	*pResult = 0;

	NMLISTVIEW* nm = (NMLISTVIEW*)pNMHDR;

	int index = nm->iItem;
	if (index < 0 || mParam.mItems.size() <= index) {
		return;
	}

	auto& item = mParam.mItems[index];

	if (nm->iSubItem == 1) {
		item.mIsWait = !item.mIsWait;
		SetItemToList(index, item);
	}
	else {
		return;
	}
}


void GroupEditDialog::OnNotifyItemDblClk(NMHDR *pNMHDR, LRESULT *pResult)
{
	ASSERT(mCommandListPtr);

	*pResult = 0;

	NMLISTVIEW* nm = (NMLISTVIEW*)pNMHDR;

	int index = nm->iItem;
	if (index < 0 || mParam.mItems.size() <= index) {
		return;
	}

	auto& item = mParam.mItems[index];

	if (nm->iSubItem == 0) {
		if (SelectCommand(index) == false) {
			return;
		}
		UpdateStatus();
		UpdateData(FALSE);
	}
	else {
		return;
	}
}

} // end of namespace group
} // end of namespace commands
} // end of namespace soyokaze

