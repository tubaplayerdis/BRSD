#include "../../../Include/Modules/Command Engine/commands.h"
#include "../../../Include/Global/global.h"

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
	if (args.size() < 1) { messages::ToFewArgs(info, "/kill", Command::get_command_string(Moderation)); return false; }
	SDK::ABrickPlayerController* controller = static_cast<SDK::ABrickPlayerController*>(GetBrickPlayerControllerFromIDORName(args[0]));
	controller->KillCharacter();
	messages::sendUserSpecificMessage(info, "Killed Player: " + GetPlayerNameFromIDORName(args[0]));
	return true;
}

bool revive_command_impl(PlayerInfo info, std::vector<std::string> args)
{
	if (args.size() < 1) { messages::ToFewArgs(info, "/revive", Command::get_command_string(Moderation)); return false; }
	SDK::ABrickPlayerController* controller = static_cast<SDK::ABrickPlayerController*>(GetBrickPlayerControllerFromIDORName(args[0]));
	SDK::FTransform transform = controller->Character->GetTransform();
	SDK::ABrickGameMode* game_mode = SDK::ABrickGameMode::Get(World());
	game_mode->RestartPlayerAtTransform(controller, transform);
	messages::sendUserSpecificMessage(info, "Revived Player: " + GetPlayerNameFromIDORName(args[0]));
	return true;
}

bool empty_command_impl(PlayerInfo info, std::vector<std::string> args)
{
	if (args.size() < 1)
	{ 
		messages::ToFewArgs(info, "/empty", Command::get_command_string(Moderation));
		return false;
	}
	SDK::ABrickPlayerController* controller = static_cast<SDK::ABrickPlayerController*>(GetBrickPlayerControllerFromIDORName(args[0]));
	if (!controller || !controller->Character || !controller->Character->IsA(SDK::ABrickCharacter::StaticClass())) return false;

	SDK::ABrickCharacter* character = static_cast<SDK::ABrickCharacter*>(controller->Character);
	if (!character || !character->InventoryComponent) return false;

	//TODO: Remove the items from the player then destory them to avoid crash.

	character->InventoryComponent->EmptyInventory(true);
	messages::sendUserSpecificMessage(info, "Emptied Users Inventory: " + GetPlayerNameFromIDORName(args[0]));
	return true;
}

bool vehdel_command_impl(PlayerInfo info, std::vector<std::string> args)
{
	if (args.size() < 1) { messages::ToFewArgs(info, "/vehdel", Command::get_command_string(Moderation)); return false; }
	SDK::ABrickPlayerController* controller = static_cast<SDK::ABrickPlayerController*>(GetBrickPlayerControllerFromIDORName(args[0]));
	if (controller && controller->GetPlayerVehicle())
	{
		SDK::ABrickVehicle* vehicle = controller->GetPlayerVehicle();
		static_cast<SDK::ABrickCharacter*>(controller->GetPlayerCharacter())->ForceEjectFromVehicle();
		vehicle->ScrapVehicle();
	}
	messages::sendUserSpecificMessage(info, "Destroyed Players Vehicle: " + GetPlayerNameFromIDORName(args[0]));
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
COMMAND_ADMIN_IMMUTABLE(Moderation, revive, "Revive a player", revive_command_impl);
COMMAND_ADMIN_IMMUTABLE(Moderation, empty, "Emptys a player's inventory", empty_command_impl);
COMMAND_ADMIN_IMMUTABLE(Moderation, vehdel, "Deletes the vehicle the user currently has", vehdel_command_impl);
COMMAND_ADMIN_IMMUTABLE(Moderation, saver, "Runs the saver module and saves the vehicle states", saver_command_impl);
COMMAND_ADMIN_IMMUTABLE(Moderation, loader, "Loads the saved vehicle states based on name", loader_command_impl);
