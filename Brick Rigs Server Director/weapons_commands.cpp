#include "commands.h"

COMMAND(Weapons, ammotype, "Changes actively held weapons ammotype", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	if (args.size() < 1) { messages::ToFewArgs(info, "/ammotype", Command::get_command_string(Weapons)); return false; }
	if (interpreter::Commands::AmmoType(info, args[0]))
	{
		messages::sendUserSpecificMessage(info, "Changed ammo type on your active weapon to: " + interpreter::getAmmoTypeString(args[0]));
		return true;
	}
	return false;
});