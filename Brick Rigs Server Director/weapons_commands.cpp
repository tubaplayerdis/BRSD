#include "commands.h"

COMMAND(Weapons, ammotype, "Changes actively held weapons ammotype", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	if (args.size() < 1) return false;
	interpreter::Commands::AmmoType(info, args[0]);
	return true;
});