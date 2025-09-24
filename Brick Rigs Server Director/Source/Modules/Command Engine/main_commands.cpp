#include "../../../Include/Modules/Command Engine/commands.h"
#include <sstream>

COMMAND(Main, pm, "Personal Message Someone", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	std::stringstream str;
	for (std::string st : args)
	{
		str << st << " ";
	}
	interpreter::Commands::PersonalMessage(info, str.str());
	return true;
});

COMMAND_IMMUTABLE(Main, silence, "Disallow incoming personal messages", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	interpreter::Commands::Moderation::ToggleSilence(info, true);
	return true;
});

COMMAND_IMMUTABLE(Main, unsilence, "Allow incoming personal messages", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	interpreter::Commands::Moderation::ToggleSilence(info, false);
	return true;
});

COMMAND_IMMUTABLE(Main, block, "Block a user from personal messaging you", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	if (args.size() < 1) { messages::ToFewArgs(info, "/block", Command::get_command_string(Main)); return false; }
	interpreter::Commands::Moderation::ToggleBlock_S(info, args[0], true);
	return true;
	});

COMMAND_IMMUTABLE(Main, unblock, "Un-Block a user from personal messaging you", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	if (args.size() < 1) { messages::ToFewArgs(info, "/unblock", Command::get_command_string(Main)); return false; }
	interpreter::Commands::Moderation::ToggleBlock_S(info, args[0], false);
	return true;
});

COMMAND_IMMUTABLE(Main, pid, "List every players PlayerID", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	interpreter::Commands::Moderation::ListPlayerIDS(info);
	return true;
});

COMMAND_IMMUTABLE(Main, silenced, "Whether or not you are on silence", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	interpreter::Commands::Moderation::IsOnSilence(info);
	return true;
});