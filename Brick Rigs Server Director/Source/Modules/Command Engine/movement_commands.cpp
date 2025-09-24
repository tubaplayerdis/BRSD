#include "../../../Include/Modules/Command Engine/Commands.h"
#include "../../../Include/Global/Global.h"

COMMAND(Movement, fly, "Fly around (slow)", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	return interpreter::Commands::Fly(info);
}, "You feel lighter...");

COMMAND(Movement, walk, "Walk around", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	return interpreter::Commands::Walk(info);
}, "Your feet are heavy...");

COMMAND(Movement, tp, "Teleport to other people", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	if (args.size() < 1) { messages::ToFewArgs(info, "/tp", Command::get_command_string(Movement)); return false; }
	if (interpreter::Commands::Teleport(info, args[0]))
	{
		messages::sendUserSpecificMessage(info, "Teleported to: " + GetPlayerNameFromIDORName(args[0]));
		return true;
	}
	return false;
});

COMMAND(Movement, ghost, "Fly around and disable collisions", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	return interpreter::Commands::Ghost(info);
});