#include "commands.h"

COMMAND_ADMIN_IMMUTABLE(Moderation, on, "Enables Command", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	if (args.size() < 1) { messages::ToFewArgs(info, "/on", Command::get_command_string(Moderation)); return false; }
	interpreter::Commands::Toggle(info, args[0], true);
	return true;
});

COMMAND_ADMIN_IMMUTABLE(Moderation, off, "Disables Command", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	if (args.size() < 1) { messages::ToFewArgs(info, "/off", Command::get_command_string(Moderation)); return false; }
	interpreter::Commands::Toggle(info, args[0], false);
	return true;
});

COMMAND_ADMIN_IMMUTABLE(Moderation, mute, "Disable a users ability to message", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	if (args.size() < 1) { messages::ToFewArgs(info, "/mute", Command::get_command_string(Moderation)); return false; }
	interpreter::Commands::Moderation::ToggleMute_S(info, args[0], true);
	return true;
});

COMMAND_ADMIN_IMMUTABLE(Moderation, unmute, "Re-Enable a users ability to message", [](PlayerInfo info, std::vector<std::string> args) -> bool {
	if (args.size() < 1) { messages::ToFewArgs(info, "/unmute", Command::get_command_string(Moderation)); return false; }
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

bool kill_command_impl(PlayerInfo info, std::vector<std::string> args)
{
	messages::sendUserSpecificMessage(info, "This command has not been implemented yet!");
	return true;
}

bool revive_command_impl(PlayerInfo info, std::vector<std::string> args)
{
	messages::sendUserSpecificMessage(info, "This command has not been implemented yet!");
	return true;
}

bool empty_command_impl(PlayerInfo info, std::vector<std::string> args)
{
	messages::sendUserSpecificMessage(info, "This command has not been implemented yet!");
	return true;
}

bool vehdel_command_impl(PlayerInfo info, std::vector<std::string> args)
{
	messages::sendUserSpecificMessage(info, "This command has not been implemented yet!");
	return true;
}

bool saver_command_impl(PlayerInfo info, std::vector<std::string> args)
{
	messages::sendUserSpecificMessage(info, "This command has not been implemented yet!");
	return true;
}

bool loader_command_impl(PlayerInfo info, std::vector<std::string> args)
{
	messages::sendUserSpecificMessage(info, "This command has not been implemented yet!");
	return true;
}

COMMAND_ADMIN_IMMUTABLE(Moderation, kill, "Kill a player", kill_command_impl);
COMMAND_ADMIN_IMMUTABLE(Moderation, revive, "Revive a player", kill_command_impl);
COMMAND_ADMIN_IMMUTABLE(Moderation, empty, "Clears a player's inventory", kill_command_impl);
COMMAND_ADMIN_IMMUTABLE(Moderation, vehdel, "Deletes vehicles spawned by that player", kill_command_impl);
COMMAND_ADMIN_IMMUTABLE(Moderation, saver, "Runs the saver module and saves the vehicle states", kill_command_impl);
COMMAND_ADMIN_IMMUTABLE(Moderation, loader, "Loads the saved vehicle states based on name", kill_command_impl);
