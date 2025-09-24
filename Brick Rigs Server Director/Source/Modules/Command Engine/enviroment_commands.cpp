#include "../../../Include/Modules/Command Engine/commands.h"

COMMAND(Enviroment, night, "Make it night", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	return interpreter::Commands::Night(info);
}, "Set the time to night!");

COMMAND(Enviroment, day, "Make it day", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	return interpreter::Commands::Day(info);
}, "Set the time to day!");

COMMAND(Enviroment, rain, "Make it rain", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	return interpreter::Commands::Rain(info);
}, "Set the weather to rain!");

COMMAND(Enviroment, sun, "Make it sunny", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	return interpreter::Commands::Sun(info);
}, "Set the weather to sunny!");