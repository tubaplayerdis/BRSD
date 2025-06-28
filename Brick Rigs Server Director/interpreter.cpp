/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     interpreter.cpp                                             */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "interpreter.h"
#include "messages.h"
#include "stringlist.h"
#include <thread>
#include <functional>
#include <SDK.hpp>
#include "global.h"
#include "PlayerInfo.h"
#include "main.h"
#include "moderation.h"

#define MIF(cond, info, message) (cond ? sendUserSpecificMessage(info, message) : (void)0)

//BACKWARDS COMPATIBLITY MACROS
#define sendUserSpecificMessage(info, message) messages::sendUserSpecificMessage(info, message)
#define sendUserSpecificMessageCommandFailed(info, message) messages::sendUserSpecificMessageCommandFailed(info, message)
#define sendUserSpecificMessageWithContext(info, message, context, sender) messages::sendUserSpecificMessageWithContext(info, message, context, sender)
#define sendMessageToAdmin(message) MessageHost(message, L"BSRD")
#define ToFewArgs(info, commandstr, group) messages::ToFewArgs(info, commandstr, group)

bool interpreter::Commands::Night(PlayerInfo info)
{
    if (!isNight) { sendUserSpecificMessageCommandFailed(info, "The /night command is currently disabled!"); RETF; }
    using namespace global;
    auto cur = GetBrickGameState()->GetMatchSettings();
    std::cout << cur.WorldSetupParams.TimeOfDay << std::endl;
    cur.WorldSetupParams.TimeOfDay = 0.00f;
    GetBrickGameState()->SetMatchSettings(cur);
    RETT;
}

bool interpreter::Commands::Day(PlayerInfo info)
{
    if (!isDay) { sendUserSpecificMessageCommandFailed(info, "The /day command is currently disabled!"); RETF; }
    using namespace global;
    SDK::FMatchSettings cur = GetBrickGameState()->MatchSettings;
    std::cout << cur.WorldSetupParams.TimeOfDay << std::endl;
    cur.WorldSetupParams.TimeOfDay = 12.00f;
    GetBrickGameState()->SetMatchSettings(cur);
    RETT;
}

bool interpreter::Commands::Rain(PlayerInfo info)
{
    if (!isRain) { sendUserSpecificMessageCommandFailed(info, "The /rain command is currently disabled!"); RETF; }
    using namespace global;
    SDK::UBrickAssetManager* manager = GetBrickAssetManager();
    SDK::AWorldSetupActor* setup = GetWorldSetupActor();
    for (SDK::UWeatherCondition* condition : manager->WeatherConditions) {
        std::cout << condition->GetName() << std::endl;
        if (condition->GetName() == "WC_Rain") {
            std::cout << "FOUND Condition!" << std::endl;
            SDK::FMatchSettings cur = GetBrickGameState()->GetMatchSettings();
            cur.WorldSetupParams.Weather = condition;
            bool old = cur.bFadeIn;
            cur.bFadeIn = false;
            GetBrickGameState()->SetMatchSettings(cur);
            cur.bFadeIn = old;
        }
    }
    RETT;
}

bool interpreter::Commands::Sun(PlayerInfo info)
{
    if (!isSun) { sendUserSpecificMessageCommandFailed(info, "The /sun command is currently disabled!"); RETF; }
    using namespace global;
    SDK::UBrickAssetManager* manager = GetBrickAssetManager();
    SDK::AWorldSetupActor* setup = GetWorldSetupActor();
    for (SDK::UWeatherCondition* condition : manager->WeatherConditions) {
        std::cout << condition->GetName() << std::endl;
        if (condition->GetName() == "WC_Sun") {
            std::cout << "FOUND Condition!" << std::endl;
            SDK::FMatchSettings cur = GetBrickGameState()->GetMatchSettings();
            cur.WorldSetupParams.Weather = condition;
            bool old = cur.bFadeIn;
            cur.bFadeIn = false;
            GetBrickGameState()->SetMatchSettings(cur);
            cur.bFadeIn = old;
        }
    }
    RETT;
}

bool interpreter::Commands::AmmoType(PlayerInfo info, std::string ammotype)
{
    if (!isAmmoType) { sendUserSpecificMessageCommandFailed(info, "The /ammotype command is currently disabled!"); RETF; }

    SDK::EAmmoType selected = SDK::EAmmoType::Default;
    int desired = -1;
    try {
        desired = std::stoi(ammotype);
        if (desired > 5 || desired < 0) {
            sendUserSpecificMessageCommandFailed(info, "Please use a value 0-5 for the ammo type. Ammo types are listed here: /help weapons");
            RETF;
        }
    }
    catch (...) {
        sendUserSpecificMessageCommandFailed(info, "Please use a value 0-5 for the ammo type. Ammo types are listed here: /help weapons");
        RETF;
    }
    switch (desired)
    {
    case 0:
        selected = SDK::EAmmoType::Default;
        break;
    case 1:
        selected = SDK::EAmmoType::Incendiary;
        break;
    case 2:
        selected = SDK::EAmmoType::HighExplosive;
        break;
    case 3:
        selected = SDK::EAmmoType::TargetSeeking;
        break;
    case 4:
        selected = SDK::EAmmoType::Guided;
        break;
    case 5:
        selected = SDK::EAmmoType::Flare;
        break;
    default:
        selected = SDK::EAmmoType::Default;
        break;
    }
    if (!GetBrickPlayerControllerFromName(info.name)->Character) { sendUserSpecificMessageCommandFailed(info, "A valid character to access your active item was not found! This normally happens because you are inside a vehicle."); RETF; }
    SDK::ABrickCharacter* Character = static_cast<SDK::ABrickCharacter*>(GetBrickPlayerControllerFromName(info.name)->Character);
    SDK::AInventoryItem* CItem = Character->GetCurrentItem();
    if (!CItem) { sendUserSpecificMessageCommandFailed(info, "You do not have a active item! Equip a weapon to your active item to change your ammotype!"); RETF; }
    if (!CItem->IsA(SDK::AFirearm::StaticClass())) { sendUserSpecificMessageCommandFailed(info, "Your active item is not a weapon! Equip a weapon as your active item to change your ammotype!"); RETF; }
    SDK::AFirearm* Firearm = static_cast<SDK::AFirearm*>(CItem);
    Firearm->FirearmComponent->FirearmState.AmmoType = selected;
    RETT;
}

size_t hash_string(const std::string& str) {
    std::hash<std::string> hasher;
    return hasher(str);
}

constexpr size_t hs(const char* str) {
    std::size_t hash = 14695981039346656037ull;
    while (*str) {
        hash ^= static_cast<unsigned char>(*str++);
        hash *= 1099511628211ull;
    }
    return hash;
}

void interpreter::interpretCommand(std::string command, std::vector<std::string> args, PlayerInfo info, std::string originalMessage)
{
    size_t hash_val = hash_string(command);

    for (std::string i : args) {
        std::cout << i << std::endl;
    }

    switch (hash_val) {
    case hs("/uninject"):
        Commands::Uninject(info);
        break;
    case hs("/help"):
        if (args.size() < 1) {
            Commands::Help(info, "master");
            break;
        }
        Commands::Help(info, args[0]);
        break;
    case hs("/info"):
        sendUserSpecificMessageWithContext(info, InfoMessage, SDK::EChatContext::Global, L"Info ABout BRSD:");
        break;
    case hs("/on"):
        if (args.size() < 1) { ToFewArgs(info, "/on", "moderation"); break; }
        Commands::Toggle(info, args[0], true);
        break;
    case hs("/off"):
        if (args.size() < 1) { ToFewArgs(info, "/off", "moderation"); break; }
        Commands::Toggle(info, args[0], false);
        break;
    case hs("/night"):
        MIF(Commands::Night(info), info, "Set the time to night!");
        break;
    case hs("/day"):
        MIF(Commands::Day(info), info, "Set the time to day!");
        break;
    case hs("/rain"):
        MIF(Commands::Rain(info), info, "Set the weather to rain!");
        break;
    case hs("/sun"):
        MIF(Commands::Sun(info), info, "Set the weather to sunny!");
        break;
    case hs("/fly"):
        MIF(Commands::Fly(info), info, "You feel lighter...");
        break;
    case hs("/walk"):
        MIF(Commands::Walk(info), info, "Your feet are heavy...");
        break;
    case hs("/pm"):
        Commands::PersonalMessage(info, originalMessage);
        break;
    case hs("/mute"):
        if (args.size() < 1) { ToFewArgs(info, "/mute", "moderation"); break; }
        Commands::Moderation::ToggleMute(info, originalMessage, true);
        break;
    case hs("/unmute"):
        if (args.size() < 1) { ToFewArgs(info, "/unmute", "moderation"); break; }
        Commands::Moderation::ToggleMute(info, originalMessage, false);
        break;
    case hs("/silence"):
        Commands::Moderation::ToggleSilence(info, true);
        break;
    case hs("/unsilence"):
        Commands::Moderation::ToggleSilence(info, false);
        break;
    case hs("/block"):
        if (args.size() < 1) { ToFewArgs(info, "/block", "main"); break; }
        Commands::Moderation::ToggleBlock(info, originalMessage, true);
        break;
    case hs("/unblock"):
        if (args.size() < 1) { ToFewArgs(info, "/unblock", "main"); break; }
        Commands::Moderation::ToggleBlock(info, originalMessage, false);
        break;
    case hs("/ghost"):
        MIF(Commands::Ghost(info), info, "Set the movement mode to ghost!");
        break;
    case hs("/save"):
        Commands::Moderation::Save(info);
        break;
    case hs("/load"):
        Commands::Moderation::Load(info);
        break;
    case hs("/blocked"):
        Commands::Moderation::ListBlocked(info);
        break;
    case hs("/muted"):
        Commands::Moderation::ListMuted(info);
        break;
    case hs("/silenced"):
        Commands::Moderation::IsOnSilence(info);
        break;
    case hs("/pid"):
        Commands::Moderation::ListPlayerIDS(info);
        break;
    case hs("/ammotype"):
        if (args.size() < 1) { ToFewArgs(info, "/ammotype", "weapons"); break; }
        MIF(Commands::AmmoType(info, args[0]), info, "Changed ammo type on your active weapon to: " + getAmmoTypeString(args[0]));
        break;
    case hs("/tp"):
        if (args.size() < 1) { ToFewArgs(info, "/tp", "movement"); break; }
        MIF(Commands::Teleport(info, args[0]), info, "Teleported to: " + GetPlayerNameFromIDORName(args[0]));
        break;
    default:
        sendUserSpecificMessageCommandFailed(info, "The command: " + command + " was not found! Use /help to view all commands!");
        break;
    }
}

std::string interpreter::getAmmoTypeString(std::string ammotype)
{
    SDK::EAmmoType selected = SDK::EAmmoType::Default;
    int desired = -1;
    try {
        desired = std::stoi(ammotype);
        if (desired > 5 || desired < 0) {
            return std::string("N/A");
        }
    }
    catch (...) {
        return std::string("N/A");
    }

    switch (desired)
    {
    case 0:
        selected = SDK::EAmmoType::Default;
        break;
    case 1:
        selected = SDK::EAmmoType::Incendiary;
        break;
    case 2:
        selected = SDK::EAmmoType::HighExplosive;
        break;
    case 3:
        selected = SDK::EAmmoType::TargetSeeking;
        break;
    case 4:
        selected = SDK::EAmmoType::Guided;
        break;
    case 5:
        selected = SDK::EAmmoType::Flare;
        break;
    default:
        selected = SDK::EAmmoType::Default;
        break;
    }

    switch (selected)
    {
    case SDK::EAmmoType::Default:
        return std::string("Default");
    case SDK::EAmmoType::Incendiary:
        return std::string("Incendiary");
    case SDK::EAmmoType::HighExplosive:
        return std::string("HighExplosive");
    case SDK::EAmmoType::TargetSeeking:
        return std::string("TargetSeeking");
    case SDK::EAmmoType::Guided:
        return std::string("Guided");
    case SDK::EAmmoType::Flare:
        return std::string("Flare");
    case SDK::EAmmoType::Max:
        return std::string("Max");
    default:
        return std::string("N/A");
    }
}

void interpreter::Commands::Toggle(PlayerInfo info, std::string command, bool toggle)
{
    using namespace global;
    if (!GetIsPlayerAdminFromName(info.name)) {
        sendUserSpecificMessageCommandFailed(info, "Only admins can use this command!");
        return;
    }
    size_t hash_val = hash_string(command);
    bool defa = false;

    switch (hash_val) {
    case hs("night"):
        isNight = toggle;
        break;
    case hs("day"):
        isDay = toggle;
        break;
    case hs("rain"):
        isRain = toggle;
        break;
    case hs("sun"):
        isSun = toggle;
        break;
    case hs("fly"):
        isFly = toggle;
        break;
    case hs("walk"):
        isWalk = toggle;
        break;
    case hs("ghost"):
        isGhost = toggle;
        break;
    case hs("ammotype"):
        isAmmoType = toggle;
        break;
    case hs("tp"):
        isTeleport = toggle;
        break;
    default:
        defa = true;
        break;
    }
    if (!defa) sendUserSpecificMessage(info, "Toggled: " + command + " to" + GetBoolString(toggle));
}

/*
* Preconditions:
* The user has managed to correctly input /pm and a space
* originalMessage is the raw original message
*/
void interpreter::Commands::PersonalMessage(PlayerInfo info, std::string originalMessage)
{
    if (!isPM) { sendUserSpecificMessageCommandFailed(info, "The /pm command is currently disabled!"); return; }

    size_t firstSpace = originalMessage.find_first_of(' ');
    if (firstSpace == std::string::npos) { sendUserSpecificMessageCommandFailed(info, "There was a formatting error when using /pm! Usage Example: /pm john123 Whats Up!"); return; } //This realisitclly shouldnt happen, but edge cases are edge cases

    std::string sub = originalMessage.substr(firstSpace + 1);
    size_t second = sub.find_first_of(' ');
    if (second == std::string::npos) { sendUserSpecificMessageCommandFailed(info, "There was a formatting error when using /pm! use a space after the intended recipient!"); return; }

    std::string recipient = sub.substr(0, second);

    SDK::ABrickPlayerController* otherCont = GetBrickPlayerControllerFromName(recipient);
    if (!otherCont) otherCont = GetBrickPlayerControllerFromID(recipient);
    if (!otherCont) { sendUserSpecificMessageCommandFailed(info, "The intended recipient was not found! Please try agian."); return; }
    PlayerInfo recipientInfo = GetPlayerInfoFromController(otherCont);

    //Send the user back thier original message for context.
    std::wstring UserContextMessage = L"Only you and " + global::to_wstring_n(recipientInfo.name) + L" can see this)";
    sendUserSpecificMessageWithContext(info, originalMessage, SDK::EChatContext::Global, UserContextMessage.c_str());

    if (moderation::isPlayerBlockedBy(info, PlayerInfo(recipient))) { sendUserSpecificMessageCommandFailed(info, "You cannot message this user!"); return; }
    if (moderation::isPlayerOnSilence(info)) { sendUserSpecificMessageCommandFailed(info, "You cannot message this user!"); return; }

    std::string message = sub.substr(second + 1);
    std::wstring contextmessage = global::to_wstring_n(info.name);
    contextmessage += L" (Personal Message)";
    sendUserSpecificMessageWithContext(PlayerInfo(recipientInfo), message, SDK::EChatContext::Global, contextmessage.c_str());

}

void interpreter::Commands::Help(PlayerInfo info, std::string arg)
{
    size_t hash_val = hash_string(arg);
    switch (hash_val) {
    case hs("controls"):
        sendUserSpecificMessageWithContext(info, ControlsHelpMessage, SDK::EChatContext::Global, L"Controls List");
        break;
    case hs("master"):
        sendUserSpecificMessageWithContext(info, MasterHelpMessage, SDK::EChatContext::Global, L"Help Command List");
        break;
    case hs("main"):
        sendUserSpecificMessageWithContext(info, MainHelpMessage, SDK::EChatContext::Global, L"Main Command List");
        break;
    case hs("moderation"):
        sendUserSpecificMessageWithContext(info, ModerationHelpMessage, SDK::EChatContext::Global, L"Moderation Command List");
        break;
    case hs("movement"):
        sendUserSpecificMessageWithContext(info, MovementHelpMessage, SDK::EChatContext::Global, L"Movement Command List");
        break;
    case hs("enviroment"):
        sendUserSpecificMessageWithContext(info, EnviromentHelpMessage, SDK::EChatContext::Global, L"Enviroment Command List");
        break;
    case hs("weapons"):
        sendUserSpecificMessageWithContext(info, WeaponsHelpMessage, SDK::EChatContext::Global, L"Weapons Command List");
        break;
    default:
        sendUserSpecificMessageWithContext(info, MasterHelpMessage, SDK::EChatContext::Global, L"Help Command List");
        break;
    }
}

//Verifies that the playercontrollers character and character movement are not null.
bool canModifyMovement(SDK::ABrickPlayerController* Controller)
{
    return (Controller->Character != nullptr && Controller->Character->CharacterMovement != nullptr);
}

bool interpreter::Commands::Fly(PlayerInfo info)
{
    if (!isFly) { sendUserSpecificMessageCommandFailed(info, "The /fly command is currently disabled!"); RETF; }
    using namespace global;
    auto BrickPlayerController = GetBrickPlayerControllerFromName(info.name);
    if (BrickPlayerController == nullptr) { sendUserSpecificMessageCommandFailed(info, "Your ABrickPlayerController was not found. Cannot change movement modes"); RETF; }
    if (GetBrickPlayerControllerFromName(info.name)) GetBrickPlayerControllerFromName(info.name)->Character->SetActorEnableCollision(true);
    if (canModifyMovement(BrickPlayerController)) { BrickPlayerController->Character->CharacterMovement->SetMovementMode(SDK::EMovementMode::MOVE_Flying, 0); RETT; }
    else sendUserSpecificMessageCommandFailed(info, "Movement commands can only be used when controlling an independent character (not in a vehicle).");
    RETF;
    //Change accel and speed values to appropriate levels
}

bool interpreter::Commands::Walk(PlayerInfo info)
{
    using namespace global;
    if (!isWalk) { sendUserSpecificMessageCommandFailed(info, "The /walk command is currently disabled!"); RETF; }
    auto BrickPlayerController = GetBrickPlayerControllerFromName(info.name);
    if (BrickPlayerController == nullptr) { sendUserSpecificMessageCommandFailed(info, "Your ABrickPlayerController was not found. Cannot change movement modes"); RETF; }
    if (GetBrickPlayerControllerFromName(info.name)) GetBrickPlayerControllerFromName(info.name)->Character->SetActorEnableCollision(true);
    if (canModifyMovement(BrickPlayerController)) {
        BrickPlayerController->Character->CharacterMovement->MaxAcceleration = 750; //This is the default value
        BrickPlayerController->Character->CharacterMovement->SetMovementMode(SDK::EMovementMode::MOVE_Walking, 0);
        RETT;
    }
    else sendUserSpecificMessageCommandFailed(info, "Movement commands can only be used when controlling an independent character (not in a vehicle).");
    RETF;
    //Return speed and accel to regular values
}

bool interpreter::Commands::Teleport(PlayerInfo info, std::string other)
{
    if (!isTeleport) { sendUserSpecificMessageCommandFailed(info, "The /tp command is currently disabled!"); RETF; }
    SDK::ABrickPlayerController* controllerother = GetBrickPlayerControllerFromIDORName(other);
    if (controllerother == nullptr) { sendUserSpecificMessageCommandFailed(info, "The user you wanted to teleport to was not found!"); RETF; }
    if (controllerother->K2_GetPawn() == nullptr) { sendUserSpecificMessageCommandFailed(info, "The user you wanted to teleport to was not in a valid location!"); RETF; }
    SDK::ABrickPlayerController* controller = GetBrickPlayerControllerFromName(info.name);
    if (!canModifyMovement(controller)) { sendUserSpecificMessageCommandFailed(info, "Movement commands can only be used when controlling an independent character (not in a vehicle)."); RETF; }

    controller->Character->K2_SetActorLocation(controllerother->K2_GetPawn()->K2_GetActorLocation(), false, nullptr, true);
    RETT;
}

bool interpreter::Commands::Ghost(PlayerInfo info)
{
    if (!isGhost) { sendUserSpecificMessageCommandFailed(info, "The /ghost command is currently disabled!"); RETF; }
    using namespace global;
    auto BrickPlayerController = GetBrickPlayerControllerFromName(info.name);
    if (BrickPlayerController == nullptr) { sendUserSpecificMessageCommandFailed(info, "Your ABrickPlayerController was not found. Cannot change movement modes"); RETF; }
    if (canModifyMovement(BrickPlayerController)) {
        BrickPlayerController->Character->SetActorEnableCollision(false);
        BrickPlayerController->Character->CharacterMovement->SetMovementMode(SDK::EMovementMode::MOVE_Flying, 0);
        RETT;
    }
    else sendUserSpecificMessageCommandFailed(info, "Movement commands can only be used when controlling an independent character (not in a vehicle).");
    RETF;
}

/*
Replace code with varius things.
*/
void interpreter::Commands::Debug(PlayerInfo info)
{
    sendMessageToAdmin("This message is really sad!");
}

void interpreter::Commands::Uninject(PlayerInfo info)
{
    if (!GetIsPlayerHostFromName(info.name)) { sendUserSpecificMessageCommandFailed(info, "Only hosts can use this command!"); return; }
    doUninject = true;
}

/*
* FailedToSpawnVehicleFormat
* Failed to spawn your vehicle
* VehicleSpawnFail
* ProjectileCam
* MoneyPlus
* MoneyMinus
* VehicleCam
* FirstPersonCamera
* VehicleCamera
* First Person
* ThirdPersonCamera
* Third Person
* save
* CameraSpeed
* zoom
* slomo
*/

void interpreter::Commands::Moderation::ToggleMute(PlayerInfo info, std::string originalMessage, bool on_off)
{
    using namespace moderation;
    if (!GetIsPlayerAdminFromName(info.name)) { sendUserSpecificMessageCommandFailed(info, "Only admins can use this command!"); return; }
    std::string input = originalMessage.substr(originalMessage.find_first_of(' ') + 1);
    SDK::ABrickPlayerController* controller = nullptr;
    controller = GetBrickPlayerControllerFromName(input);
    if (!controller) controller = GetBrickPlayerControllerFromID(input);
    if (!controller) { sendUserSpecificMessageCommandFailed(info, "The user you wanted to mute was not found!"); return; }
    PlayerInfo other = GetPlayerInfoFromController(controller);
    if (info == other) { sendUserSpecificMessageCommandFailed(info, "You cannot mute yourself!"); return; }
    if (on_off) {
        if (!AddMutedPlayer(other)) sendUserSpecificMessageWithContext(info, other.name + " is already muted!", SDK::EChatContext::Admin, L"Admin");
        else sendUserSpecificMessageWithContext(info, std::string("Successfully Muted: ") + other.name, SDK::EChatContext::Admin, L"Admin");
    }
    else {
        if (!RemoveMutedPlayer(other)) sendUserSpecificMessageWithContext(info, other.name + "is already unmuted", SDK::EChatContext::Admin, L"Admin");
        else sendUserSpecificMessageWithContext(info, std::string("Successfully Unmuted: ") + other.name, SDK::EChatContext::Admin, L"Admin");
    }
}

void interpreter::Commands::Moderation::ToggleBlock(PlayerInfo info, std::string originalMessage, bool on_off)
{
    using namespace moderation;
    std::string input = originalMessage.substr(originalMessage.find_first_of(' ') + 1);
    SDK::ABrickPlayerController* controller = nullptr;
    controller = GetBrickPlayerControllerFromName(input);
    if (!controller) controller = GetBrickPlayerControllerFromID(input);
    if (!controller) { sendUserSpecificMessageCommandFailed(info, "The user you wanted to block was not found!"); return; }
    PlayerInfo other = GetPlayerInfoFromController(controller);
    if (on_off) {
        if (!AddBlockedPlayer(BlockedPlayer(info, other))) sendUserSpecificMessageWithContext(info, std::string("You have already blocked: ") + other.name, SDK::EChatContext::Admin, L"Admin");
        else sendUserSpecificMessageWithContext(info, std::string("Successfully Blocked: ") + other.name, SDK::EChatContext::Admin, L"Admin");
    }
    else {
        if (!RemoveBlockedPlayer(BlockedPlayer(info, other)))  sendUserSpecificMessageWithContext(info, std::string("You have already unblocked: ") + other.name, SDK::EChatContext::Admin, L"Admin");
        else sendUserSpecificMessageWithContext(info, std::string("Successfully Unblocked: ") + other.name, SDK::EChatContext::Admin, L"Admin");
    }
}

void interpreter::Commands::Moderation::ToggleSilence(PlayerInfo info, bool on_off)
{
    using namespace moderation;
    if (on_off) {
        if (!AddPMSilencePlayer(info)) sendUserSpecificMessageWithContext(info, std::string("You are already on silence!"), SDK::EChatContext::Global, L"BRCI Moderation");
        else sendUserSpecificMessageWithContext(info, std::string("Disallowed incoming personal messages!"), SDK::EChatContext::Global, L"BRCI Moderation");
    }
    else {
        if (!RemovePMSilencePlayer(info)) sendUserSpecificMessageWithContext(info, std::string("You are already not on silence!"), SDK::EChatContext::Global, L"BRCI Moderation");
        else sendUserSpecificMessageWithContext(info, std::string("Allowed incoming personal messages!"), SDK::EChatContext::Global, L"BRCI Moderation");
    }
}

void interpreter::Commands::Moderation::Save(PlayerInfo info)
{
    using namespace moderation;
    if (!GetIsPlayerAdminFromName(info.name)) { sendUserSpecificMessageCommandFailed(info, "Only admins can use this command!"); return; }
    if (saveModerationValues()) sendUserSpecificMessage(info, "Successfully saved the current moderation values to the disk!");
    else sendUserSpecificMessageCommandFailed(info, "Failed to save the current moderation values to the disk!");
}

void interpreter::Commands::Moderation::Load(PlayerInfo info)
{
    using namespace moderation;
    if (!GetIsPlayerAdminFromName(info.name)) { sendUserSpecificMessageCommandFailed(info, "Only admins can use this command!"); return; }
    if (loadModerationValues()) sendUserSpecificMessage(info, "Successfully loaded the current moderation values from the disk!");
    else sendUserSpecificMessageCommandFailed(info, "Failed to load the current moderation values from the disk! Either the file does not exist or it is corrupted");
}

void interpreter::Commands::Moderation::ListMuted(PlayerInfo info)
{
    using namespace moderation;
    if (!GetIsPlayerAdminFromName(info.name)) { sendUserSpecificMessageCommandFailed(info, "Only admins can use this command!"); return; }
    std::string message = "Currently Muted Players:";
    for (PlayerInfo infou : MutedPlayers) {
        message += "\n" + infou.name;
    }
    sendUserSpecificMessage(info, message);
}

void interpreter::Commands::Moderation::ListBlocked(PlayerInfo info)
{
    using namespace moderation;
    std::string message = "Currently Blocked Players:";
    for (BlockedPlayer player : BlockedPlayers) {
        if (player.Blocker == info) {
            message += "\n" + info.name;
        }
    }
    sendUserSpecificMessage(info, message);
}

void interpreter::Commands::Moderation::IsOnSilence(PlayerInfo info)
{
    using namespace moderation;
    for (PlayerInfo player : PlayersOnPMSilence) {
        if (player.name == info) {
            sendUserSpecificMessage(info, "You are on silence. You will not recieve Personal Messages(PM)");
            return;
        }
    }
    sendUserSpecificMessage(info, "You are not on silence. You will recieve Personal Messages(PM)");
}

void interpreter::Commands::Moderation::ListPlayerIDS(PlayerInfo info)
{
    std::string message = "Player IDS:";
    UC::TArray<SDK::AActor*> raw = UC::TArray<SDK::AActor*>();
    UC::TArray<SDK::AActor*>* what = &raw;
    SDK::UGameplayStatics::GetAllActorsOfClass(World(), SDK::ABrickPlayerController::StaticClass(), what);
    for (int i = 0; i < raw.Num(); i++)
    {
        SDK::ABrickPlayerController* cast = static_cast<SDK::ABrickPlayerController*>(raw[i]);
        SDK::ABrickPlayerState* state = static_cast<SDK::ABrickPlayerState*>(cast->PlayerState);
        message += "\n" + state->GetPlayerNameText().ToString() + ": " + std::to_string(state->PlayerId);
    }
    sendUserSpecificMessage(info, message);
}
