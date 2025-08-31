#include "commands.h"

COMMAND(Movement, fly, "Fly around (slow)", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	return interpreter::Commands::Fly(info);
});

COMMAND(Movement, walk, "Walk around", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	return interpreter::Commands::Walk(info);
});

COMMAND(Movement, tp, "Teleport to other people", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	if (args.size() < 1) return false;
	return interpreter::Commands::Teleport(info, args[0]);
});

COMMAND(Movement, ghost, "Fly around (slow)", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	return interpreter::Commands::Ghost(info);
});