#include "commands.h"

COMMAND_IMMUTABLE(Master, info, "Info about BRCE", [](PlayerInfo info, std::vector<std::string>) -> bool {
	messages::sendUserSpecificMessageWithContext(info, InfoMessage, SDK::EChatContext::Global, L"Info About BRSD:");
	return true;
});

COMMAND_IMMUTABLE(Master, help, "Command Lists", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	if (args.size() < 1) return false;
	interpreter::Commands::Help(info, args[0]);
	return true;
});