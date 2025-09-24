#include "../../../Include/Modules/Command Engine/Commands.h"

COMMAND_IMMUTABLE(Master, info, "Info about BRSD", [](PlayerInfo info, std::vector<std::string>) -> bool {
	messages::sendUserSpecificMessageWithContext(info, InfoMessage, SDK::EChatContext::Global, L"Info About BRSD:");
	return true;
});

COMMAND_IMMUTABLE(Master, help, "Command Lists", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	if (args.size() < 1) 
	{
		messages::sendUserSpecificMessage(info, MasterHelpMessage);
		return true;
	}
	interpreter::Commands::Help(info, args[0]);
	return true;
});

COMMAND_ADMIN_IMMUTABLE(Master, uninject, "Uninjects BRSD", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	interpreter::Commands::Uninject(info);
	return true;
})