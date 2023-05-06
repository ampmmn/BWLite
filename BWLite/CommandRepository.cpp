#include "pch.h"
#include "framework.h"
#include "CommandRepository.h"
#include "CommandMap.h"
#include "AboutDlg.h"
#include "AppProfile.h"
#include "ForwardMatchPattern.h"
#include "PartialMatchPattern.h"
#include "SkipMatchPattern.h"
#include "WholeMatchPattern.h"
#include "CommandFile.h"
#include "commands/NewCommand.h"
#include "commands/ShellExecCommand.h"
#include "commands/ReloadCommand.h"
#include "commands/EditCommand.h"
#include "commands/ExitCommand.h"
#include "commands/VersionCommand.h"
#include "commands/UserDirCommand.h"
#include "commands/MainDirCommand.h"
#include "commands/SettingCommand.h"
#include "commands/ManagerCommand.h"
#include "commands/ExecutableFileCommand.h"
#include "CommandEditDialog.h"
#include "KeywordManagerDialog.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct CommandRepository::PImpl
{
	PImpl() : mPattern(nullptr)
	{
	}

	// �ݒ�t�@�C��(commands.ini)����̓ǂݏ������s��
	CommandFile mCommandLoader;
	// �g�ݍ��݃R�}���h�ꗗ
	CommandMap mBuiltinCommands;
	// ��ʃR�}���h�ꗗ
	CommandMap mCommands;
	// ���ϐ�PATH�ɂ���exe�����s���邽�߂̃R�}���h
	ExecutableFileCommand mExeCommand;
	// �L�[���[�h��r�p�̃N���X
	Pattern* mPattern;
};


CommandRepository::CommandRepository() : in(new PImpl)
{
}

CommandRepository::~CommandRepository()
{
	delete in->mPattern;
	delete in;
}



BOOL CommandRepository::Load()
{
	// �����̓��e��j��
	in->mCommands.Clear();
	in->mBuiltinCommands.Clear();

	// �L�[���[�h��r�����̐���
	delete in->mPattern;

	CAppProfile* app = CAppProfile::Get();
	int matchLevel = app->Get(_T("BWLite"), _T("MatchLevel"), 2);
	if (matchLevel == 0) {
		// �X�L�b�v�}�b�`���O��r�p
		in->mPattern = new SkipMatchPattern();
	}
	else if (matchLevel == 1) {
		// ������v��r�p
		in->mPattern = new PartialMatchPattern();
	}
	else {
		// �O����v��r�p
		in->mPattern = new ForwardMatchPattern();
	}

	// �r���g�C���R�}���h�̓o�^
	in->mBuiltinCommands.Register(new NewCommand(this));
	in->mBuiltinCommands.Register(new EditCommand(this));
	in->mBuiltinCommands.Register(new ReloadCommand(this));
	in->mBuiltinCommands.Register(new ManagerCommand(this));
	in->mBuiltinCommands.Register(new ExitCommand());
	in->mBuiltinCommands.Register(new VersionCommand());
	in->mBuiltinCommands.Register(new UserDirCommand());
	in->mBuiltinCommands.Register(new MainDirCommand());
	in->mBuiltinCommands.Register(new SettingCommand());

	// �ݒ�t�@�C����ǂ݁A�R�}���h�ꗗ��o�^����
	TCHAR path[32768];
	CAppProfile::GetDirPath(path, 32768);
	PathAppend(path, _T("commands.ini"));
	in->mCommandLoader.SetFilePath(path);

	std::vector<Command*> commands;
	in->mCommandLoader.Load(commands);

	for (auto cmd : commands) {
		in->mCommands.Register(cmd);
	}

	return TRUE;
}


/**
 *  �V�K�L�[���[�h�쐬
 *  @param cmdNamePtr �쐬����R�}���h��(null�̏ꍇ�̓R�}���h�����󗓂ɂ���)
 */
int CommandRepository::NewCommandDialog(
	const CString* cmdNamePtr
)
{
	// �V�K�쐬�_�C�A���O��\��
	CommandEditDialog dlg(this);

	if (cmdNamePtr) {
		dlg.SetName(*cmdNamePtr);
	}

	if (dlg.DoModal() != IDOK) {
		return 0;
	}

	// �_�C�A���O�œ��͂��ꂽ���e�Ɋ�Â��A�R�}���h��V�K�쐬����
	auto* newCmd = new ShellExecCommand();
	newCmd->SetName(dlg.mName);
	newCmd->SetDescription(dlg.mDescription);
	newCmd->SetRunAs(dlg.mIsRunAsAdmin);

	ShellExecCommand::ATTRIBUTE normalAttr;
	normalAttr.mPath = dlg.mPath;
	normalAttr.mParam = dlg.mParameter;
	normalAttr.mDir = dlg.mDir;
	normalAttr.mShowType = dlg.GetShowType();
	newCmd->SetAttribute(normalAttr);

	if (dlg.mIsUse0) {
		ShellExecCommand::ATTRIBUTE param0Attr;
		param0Attr.mPath = dlg.mPath0;
		param0Attr.mParam = dlg.mParameter0;
		param0Attr.mDir = dlg.mDir;
		param0Attr.mShowType = dlg.GetShowType();
		newCmd->SetAttributeForParam0(param0Attr);
	}
	else {
		ShellExecCommand::ATTRIBUTE param0Attr;
		newCmd->SetAttributeForParam0(param0Attr);
	}

	in->mCommands.Register(newCmd);

	// �ݒ�t�@�C���ɕۑ�
	std::vector<Command*> cmdsTmp;
	in->mCommandLoader.Save(in->mCommands.Enumerate(cmdsTmp));
	return 0;
}


/**
 *  �����L�[���[�h�̕ҏW
 */
int CommandRepository::EditCommandDialog(const CString& cmdName)
{
	auto cmdAbs = in->mCommands.Get(cmdName);
	if (cmdAbs == nullptr) {
		return 1;
	}

	// ToDo: ��ŃN���X�݌v��������
	ShellExecCommand* cmd = (ShellExecCommand*)cmdAbs;

	CommandEditDialog dlg(this);
	dlg.SetOrgName(cmdName);

	dlg.mName = cmd->GetName();
	dlg.mDescription = cmd->GetDescription();
	dlg.mIsRunAsAdmin = cmd->GetRunAs();

	ShellExecCommand::ATTRIBUTE attr;
	cmd->GetAttribute(attr);

	dlg.mPath = attr.mPath;
	dlg.mParameter = attr.mParam;
	dlg.mDir = attr.mDir;
	dlg.SetShowType(attr.mShowType);

	cmd->GetAttributeForParam0(attr);
	dlg.mIsUse0 = (attr.mPath.IsEmpty() == FALSE);
	dlg.mPath0 = attr.mPath;
	dlg.mParameter0 = attr.mParam;

	if (dlg.DoModal() != IDOK) {
		return TRUE;
	}

	ShellExecCommand* cmdNew = new ShellExecCommand();

	// �ǉ����鏈��
	cmdNew->SetName(dlg.mName);
	cmdNew->SetDescription(dlg.mDescription);
	cmdNew->SetRunAs(dlg.mIsRunAsAdmin);

	ShellExecCommand::ATTRIBUTE normalAttr;
	normalAttr.mPath = dlg.mPath;
	normalAttr.mParam = dlg.mParameter;
	normalAttr.mDir = dlg.mDir;
	normalAttr.mShowType = dlg.GetShowType();
	cmdNew->SetAttribute(normalAttr);

	if (dlg.mIsUse0) {
		ShellExecCommand::ATTRIBUTE param0Attr;
		param0Attr.mPath = dlg.mPath0;
		param0Attr.mParam = dlg.mParameter0;
		param0Attr.mDir = dlg.mDir;
		param0Attr.mShowType = dlg.GetShowType();
		cmdNew->SetAttributeForParam0(param0Attr);
	}
	else {
		ShellExecCommand::ATTRIBUTE param0Attr;
		cmdNew->SetAttributeForParam0(param0Attr);
	}

	// ���O���ς���Ă���\�������邽�߁A��������폜���čēo�^����
	in->mCommands.Unregister(cmd);
	in->mCommands.Register(cmdNew);

	// �t�@�C���ɕۑ�
	std::vector<Command*> cmdsTmp;
	in->mCommandLoader.Save(in->mCommands.Enumerate(cmdsTmp));

	return 0;
}

/**
 * �w�肵���R�}���h���͑g�ݍ��݃R�}���h��?
 */
bool CommandRepository::IsBuiltinName(const CString& cmdName)
{
	return in->mBuiltinCommands.Has(cmdName);
}

/**
 *  �L�[���[�h�}�l�[�W���[�̕\��
 */
int CommandRepository::ManagerDialog()
{
	// �L�����Z�����p�̃o�b�N�A�b�v
	CommandMap builtinBkup(in->mBuiltinCommands);
	CommandMap commandsBkup(in->mCommands);

	KeywordManagerDialog dlg(this);

	if (dlg.DoModal() != IDOK) {

		// OK�ł͂Ȃ��̂Ō��ʂ𔽉f���Ȃ�(�o�b�N�A�b�v�������e�ɖ߂�)
		in->mBuiltinCommands.Swap(builtinBkup);
		in->mCommands.Swap(commandsBkup);
	}
	else {
		// �t�@�C���ɕۑ�
		std::vector<Command*> cmdsTmp;
		in->mCommandLoader.Save(in->mCommands.Enumerate(cmdsTmp));
	}
	return 0;
}

/**
 *  �R�}���h�̍폜
 */
bool CommandRepository::DeleteCommand(const CString& cmdName)
{
	return in->mCommands.Unregister(cmdName);
}

void CommandRepository::EnumCommands(std::vector<Command*>& enumCommands)
{
	enumCommands.clear();
	in->mCommands.Enumerate(enumCommands);
	in->mBuiltinCommands.Enumerate(enumCommands);
}

void
CommandRepository::Query(
	const CString& strQueryStr,
	std::vector<Command*>& items
)
{
	items.clear();

	in->mPattern->SetPattern(strQueryStr);

	in->mCommands.Query(in->mPattern, items);
	in->mBuiltinCommands.Query(in->mPattern, items);

	// 1�����}�b�`���Ȃ��ꍇ��ExecutableCommand�̂Ђ���
	if (items.empty()) {
		if (in->mExeCommand.Match(in->mPattern)) {
			items.push_back(&in->mExeCommand);
		}
	}
}

Command* CommandRepository::QueryAsWholeMatch(
	const CString& strQueryStr
)
{
	WholeMatchPattern pat(strQueryStr);

	auto command = in->mCommands.FindOne(&pat);
	if (command != nullptr) {
		return command;
	}

	command = in->mBuiltinCommands.FindOne(&pat);
	if (command != nullptr) {
		return command;
	}

	// 1�����}�b�`���Ȃ��ꍇ��ExecutableCommand�̂Ђ���
	if (in->mExeCommand.Match(in->mPattern)) {
		return &in->mExeCommand;
	}

	return nullptr;
}

bool CommandRepository::IsValidAsName(const CString& strQueryStr)
{
	return strQueryStr.FindOneOf(_T(" !\"\\/*;:[]|&<>,.")) == -1;
}
