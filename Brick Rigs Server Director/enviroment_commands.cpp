#include "commands.h"

COMMAND(Enviroment, night, "Make it night", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	return interpreter::Commands::Night(info);
});

COMMAND(Enviroment, day, "Make it day", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	return interpreter::Commands::Day(info);
	});

COMMAND(Enviroment, rain, "Make it rain", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	return interpreter::Commands::Rain(info);
	});

COMMAND(Enviroment, sun, "Make it sunny", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	return interpreter::Commands::Sun(info);
	});