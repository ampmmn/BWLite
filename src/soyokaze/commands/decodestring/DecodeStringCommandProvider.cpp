#include "pch.h"
#include "DecodeStringCommandProvider.h"
#include "commands/decodestring/DecodeUriCommand.h"
#include "commands/decodestring/EscapedCharCommand.h"
#include "commands/decodestring/DecodeBase64Command.h"
#include "commands/core/CommandRepository.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace launcherapp {
namespace commands {
namespace decodestring {


using CommandRepository = launcherapp::core::CommandRepository;

struct DecodeStringCommandProvider::PImpl
{
	PImpl()
	{
	}
	virtual ~PImpl()
	{
	}

	RefPtr<DecodeUriCommand> mDecodeUriCommand;
	RefPtr<EscapedCharCommand> mEscapedCharCommand;
	RefPtr<DecodeBase64Command> mDecodeBase64Command;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

REGISTER_COMMANDPROVIDER(DecodeStringCommandProvider)


DecodeStringCommandProvider::DecodeStringCommandProvider() : in(std::make_unique<PImpl>())
{
	in->mDecodeUriCommand.reset(new DecodeUriCommand());
	in->mEscapedCharCommand.reset(new EscapedCharCommand());
	in->mDecodeBase64Command.reset(new DecodeBase64Command());
}

DecodeStringCommandProvider::~DecodeStringCommandProvider()
{
}

CString DecodeStringCommandProvider::GetName()
{
	return _T("DecodeString");
}

// 一時的なコマンドを必要に応じて提供する
void DecodeStringCommandProvider::QueryAdhocCommands(
	Pattern* pattern,
 	CommandQueryItemList& commands
)
{
	int level = in->mDecodeUriCommand->Match(pattern);
	if (level != Pattern::Mismatch) {
		commands.Add(CommandQueryItem(level, in->mDecodeUriCommand.get()));
		in->mDecodeUriCommand->AddRef();
	}

	level = in->mEscapedCharCommand->Match(pattern);
	if (level != Pattern::Mismatch) {
		commands.Add(CommandQueryItem(level, in->mEscapedCharCommand.get()));
		in->mEscapedCharCommand->AddRef();
	}
	level = in->mDecodeBase64Command->Match(pattern);
	if (level != Pattern::Mismatch) {
		commands.Add(CommandQueryItem(level, in->mDecodeBase64Command.get()));
		in->mDecodeBase64Command->AddRef();
	}
}


} // end of namespace decodestring
} // end of namespace commands
} // end of namespace launcherapp

