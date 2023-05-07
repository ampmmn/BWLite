#include "pch.h"
#include "framework.h"
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
#include <map>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CommandMap
{
public:
	CommandMap()
	{
	}
	CommandMap(const CommandMap& rhs)
	{
		for (auto item : rhs.mMap) {
			mMap[item.first] = item.second->Clone();
		}
	}
	~CommandMap()
	{
		Clear();
	}

	void Clear()
	{
		for (auto item : mMap) {
			delete item.second;
		}
	}

	bool Has(const CString& name) const
	{
		return mMap.find(name) != mMap.end();
	}
	Command* Get(const CString& name)
	{
		auto itFind = mMap.find(name);
		if (itFind == mMap.end()) {
			return nullptr;
		}
		return itFind->second;
	}

	void Register(Command* cmd)
	{
		mMap[cmd->GetName()] = cmd;
	}

	bool Unregister(Command* cmd)
	{
		return Unregister(cmd->GetName());
	}

	bool Unregister(const CString& name)
	{
		auto itFind = mMap.find(name);
		if (itFind == mMap.end()) {
			return false;
		}

		delete itFind->second;
		mMap.erase(itFind);
		return true;
	}

	void Swap(CommandMap& rhs)
	{
		mMap.swap(rhs.mMap);
	}

	void Query(Pattern* pattern, std::vector<Command*>& commands)
	{
		for (auto& item : mMap) {

			auto& command = item.second;
			if (command->Match(pattern) == FALSE) {
				continue;
			}
			commands.push_back(command);
		}
	}

	// �ŏ��Ɍ������v�f��Ԃ�
	Command* FindOne(Pattern* pattern)
	{
		for (auto& item : mMap) {

			auto& command = item.second;
			if (command->Match(pattern) == FALSE) {
				continue;
			}
			return item.second;
		}
		return nullptr;
	}

	std::vector<Command*>& Enumerate(std::vector<Command*>& commands)
	{
		commands.reserve(commands.size() + mMap.size());
		for (auto item : mMap) {
			commands.push_back(item.second);
		}
		return commands;
	}

protected:
	std::map<CString, Command*> mMap;
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct CommandRepository::PImpl
{
	PImpl() : pattern(nullptr)
	{
	}

	CommandFile commandLoader;

	CommandMap builtinCommands;
	CommandMap commands;
	ExecutableFileCommand* exeCommand;
	Pattern* pattern;
};


CommandRepository::CommandRepository() : in(new PImpl)
{
	in->exeCommand = new ExecutableFileCommand();
}

CommandRepository::~CommandRepository()
{
	delete in->pattern;
	delete in;
}



BOOL CommandRepository::Load()
{
	// �����̓��e��j��
	in->commands.Clear();
	in->builtinCommands.Clear();

	// �L�[���[�h��r�����̐���
	delete in->pattern;

	CAppProfile* app = CAppProfile::Get();
	int matchLevel = app->Get(_T("BWLite"), _T("MatchLevel"), 2);
	if (matchLevel == 0) {
		in->pattern = new SkipMatchPattern();
	}
	else if (matchLevel == 1) {
		in->pattern = new PartialMatchPattern();
	}
	else {
		in->pattern = new ForwardMatchPattern();
	}

	// �r���g�C���R�}���h�̓o�^
	in->builtinCommands.Register(new NewCommand(this));
	in->builtinCommands.Register(new EditCommand(this));
	in->builtinCommands.Register(new ReloadCommand(this));
	in->builtinCommands.Register(new ManagerCommand(this));
	in->builtinCommands.Register(new ExitCommand());
	in->builtinCommands.Register(new VersionCommand());
	in->builtinCommands.Register(new UserDirCommand());
	in->builtinCommands.Register(new MainDirCommand());
	in->builtinCommands.Register(new SettingCommand());

	// �ݒ�t�@�C����ǂ݁A�R�}���h�ꗗ��o�^����
	TCHAR path[32768];
	CAppProfile::GetDirPath(path, 32768);
	PathAppend(path, _T("commands.ini"));
	in->commandLoader.SetFilePath(path);

	std::vector<Command*> commands;
	in->commandLoader.Load(commands);

	for (auto cmd : commands) {
		in->commands.Register(cmd);
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

	in->commands.Register(newCmd);

	// �ݒ�t�@�C���ɕۑ�
	std::vector<Command*> cmdsTmp;
	in->commandLoader.Save(in->commands.Enumerate(cmdsTmp));
	return 0;
}


/**
 *  �����L�[���[�h�̕ҏW
 */
int CommandRepository::EditCommandDialog(const CString& cmdName)
{
	auto cmdAbs = in->commands.Get(cmdName);
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
	in->commands.Unregister(cmd);
	in->commands.Register(cmdNew);

	// �t�@�C���ɕۑ�
	std::vector<Command*> cmdsTmp;
	in->commandLoader.Save(in->commands.Enumerate(cmdsTmp));

	return 0;
}

/**
 * �w�肵���R�}���h���͑g�ݍ��݃R�}���h��?
 */
bool CommandRepository::IsBuiltinName(const CString& cmdName)
{
	return in->builtinCommands.Has(cmdName);
}

/**
 *  �L�[���[�h�}�l�[�W���[�̕\��
 */
int CommandRepository::ManagerDialog()
{
	// �L�����Z�����p�̃o�b�N�A�b�v
	CommandMap builtinBkup(in->builtinCommands);
	CommandMap commandsBkup(in->commands);

	KeywordManagerDialog dlg(this);

	if (dlg.DoModal() != IDOK) {

		// OK�ł͂Ȃ��̂Ō��ʂ𔽉f���Ȃ�(�o�b�N�A�b�v�������e�ɖ߂�)
		in->builtinCommands.Swap(builtinBkup);
		in->commands.Swap(commandsBkup);
	}
	else {
		// �t�@�C���ɕۑ�
		std::vector<Command*> cmdsTmp;
		in->commandLoader.Save(in->commands.Enumerate(cmdsTmp));
	}
	return 0;
}

/**
 *  �R�}���h�̍폜
 */
bool CommandRepository::DeleteCommand(const CString& cmdName)
{
	return in->commands.Unregister(cmdName);
}

void CommandRepository::EnumCommands(std::vector<Command*>& enumCommands)
{
	enumCommands.clear();
	in->commands.Enumerate(enumCommands);
	in->builtinCommands.Enumerate(enumCommands);
}

void
CommandRepository::Query(
	const CString& strQueryStr,
	std::vector<Command*>& items
)
{
	items.clear();

	in->pattern->SetPattern(strQueryStr);

	in->commands.Query(in->pattern, items);
	in->builtinCommands.Query(in->pattern, items);

	// 1�����}�b�`���Ȃ��ꍇ��ExecutableCommand�̂Ђ���
	if (items.empty()) {
		if (in->exeCommand->Match(in->pattern)) {
			items.push_back(in->exeCommand);
		}
	}
}

Command* CommandRepository::QueryAsWholeMatch(
	const CString& strQueryStr
)
{
	WholeMatchPattern pat(strQueryStr);

	auto command = in->commands.FindOne(&pat);
	if (command != nullptr) {
		return command;
	}

	command = in->builtinCommands.FindOne(&pat);
	if (command != nullptr) {
		return command;
	}

	// 1�����}�b�`���Ȃ��ꍇ��ExecutableCommand�̂Ђ���
	if (in->exeCommand->Match(in->pattern)) {
		return in->exeCommand;
	}

	return nullptr;
}

bool CommandRepository::IsValidAsName(const CString& strQueryStr)
{
	return strQueryStr.FindOneOf(_T(" !\"\\/*;:[]|&<>,.")) == -1;
}

