#pragma once

#include "commands/core/CommandProviderIF.h"


namespace launcherapp {
namespace commands {
namespace webhistory {

class WebHistoryProvider :
	public launcherapp::core::CommandProvider
{
	using Command = launcherapp::core::Command;
	using CommandParameter = launcherapp::core::CommandParameter;

private:
	WebHistoryProvider();
	virtual ~WebHistoryProvider();

public:
	// 初回起動の初期化を行う
	void OnFirstBoot() override;

	// コマンドの読み込み
	void LoadCommands(CommandFile* commandFile) override;

	CString GetName() override;

	// 作成できるコマンドの種類を表す文字列を取得
	CString GetDisplayName() override;

	// コマンドの種類の説明を示す文字列を取得
	CString GetDescription() override;

	// コマンド新規作成ダイアログ
	bool NewDialog(const CommandParameter* param) override;

	// 非公開コマンドかどうか(新規作成対象にしない)
	bool IsPrivate() const override;

	// 一時的なコマンドを必要に応じて提供する
	void QueryAdhocCommands(Pattern* pattern, CommandQueryItemList& comands) override;

	// Provider間の優先順位を表す値を返す。小さいほど優先
	uint32_t GetOrder() const override;

	// 設定ページを取得する
	bool CreateSettingPages(CWnd* parent, std::vector<SettingPage*>& pages) override;

	uint32_t AddRef() override;
	uint32_t Release() override;


	DECLARE_COMMANDPROVIDER(WebHistoryProvider)

private:
	struct PImpl;
	std::unique_ptr<PImpl> in;
};


} // end of namespace webhistory
} // end of namespace commands
} // end of namespace launcherapp

