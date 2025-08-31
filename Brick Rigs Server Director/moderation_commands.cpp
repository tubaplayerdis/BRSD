#include "commands.h"

COMMAND_ADMIN_IMMUTABLE(Moderation, on, "Enables Command", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	if (args.size() < 1) return false;
	interpreter::Commands::Toggle(info, args[0], true);
	return true;
});

COMMAND_ADMIN_IMMUTABLE(Moderation, off, "Disables Command", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	if (args.size() < 1) return false;
	interpreter::Commands::Toggle(info, args[0], false);
	return true;
});

COMMAND_ADMIN_IMMUTABLE(Moderation, mute, "Disable a users ability to message", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	if (args.size() < 1) return false;
	interpreter::Commands::Moderation::ToggleMute_S(info, args[0], true);
	return true;
});

COMMAND_ADMIN_IMMUTABLE(Moderation, unmute, "Re-Enable a users ability to message", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	if (args.size() < 1) return false;
	interpreter::Commands::Moderation::ToggleMute_S(info, args[0], false);
	return true;
});

COMMAND_ADMIN_IMMUTABLE(Moderation, save, "Save the current moderation config to the disk", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	interpreter::Commands::Moderation::Save(info);
	return true;
});

COMMAND_ADMIN_IMMUTABLE(Moderation, load, "Load the moderation config on the disk", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	interpreter::Commands::Moderation::Load(info);
	return true;
});