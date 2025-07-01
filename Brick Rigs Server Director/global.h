/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     global.h                                                    */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include "windows.h"
#include <Psapi.h>
#include <string>
#include "PlayerInfo.h"
#include <SDK.hpp>
#include <SDK/Engine_classes.hpp>

#define STRING(string) UC::FString(string)
#define NAME(text) SDK::UKismetStringLibrary::Conv_StringToName(SDK::FString(text))
#define GetBoolString(text) (text ? "true" : "false")
#define RETF return false
#define RETT return true
#define RET0 return 0
#define RET1 return 1
#define BRSD L"BRSD"
#define World() SDK::UWorld::GetWorld()
#define Engine() SDK::UEngine::GetEngine()
#define Notification(message, icon) global::SendNotificationLocal(message, icon)
#define Spawn(cls, out) static_cast<cls*>(SDK::UGameplayStatics::SpawnObject(cls::StaticClass(), out))
#define WIN32_LEAN_AND_MEAN

#undef TEXT
#define TEXT(text) SDK::UKismetTextLibrary::Conv_StringToText(SDK::FString(text))

//For "global" items, but ones that do not pollute the actual global namespace
namespace global
{
	//Useful "Global" Variables
	inline std::string mapLevelName = "None";
	inline SDK::UEngine* Engine = Engine();
	inline SDK::UWorld* World = World();
	inline SDK::ULevel* Level = World()->PersistentLevel;
	inline SDK::APlayerController* LocalController = World()->OwningGameInstance->LocalPlayers[0]->PlayerController;

	namespace pointers
	{
		inline bool updatingPointers = false;

		/// <summary>
		/// Initializes internal pointers used by the program.
		/// </summary>
		void InitPointers();

		/// <summary>
		/// Updates internal pointers to reference the specified UWorld instance.
		/// </summary>
		/// <param name="NewWorld">A pointer to the new UWorld instance to be used.</param>
		void UpdatePointers(SDK::UWorld* NewWorld);
	}

	
	/// <summary>
	/// Checks whether the map is playable.
	/// </summary>
	/// <returns>true if the map is playable; otherwise, false.</returns>
	bool isMapValid();

	/// <summary>
	/// Determines whether the specified network driver is operating as the host.
	/// </summary>
	/// <param name="driver">A pointer to the UNetDriver instance to check.</param>
	/// <returns>true if the driver is acting as the host; otherwise, false.</returns>
	bool IsHost(SDK::UNetDriver* driver);

	/// <summary>
	/// Determines whether the current world instance is the host.
	/// </summary>
	/// <returns>true if the current world instance is the host; otherwise, false.</returns>
	inline bool IsHost() { return IsHost(World()->NetDriver); }

	/// <summary>
	/// Determines whether the current world instance is not the host.
	/// </summary>
	/// <returns>true if the current instance world is not the host; otherwise, false.</returns>
	inline bool NotHost() { return !IsHost(World()->NetDriver); }
	

	/// <summary>
	/// Sends a local notification with the specified message and icon.
	/// </summary>
	/// <param name="notif">The notification message to send, as a wide string.</param>
	/// <param name="slot">The slot of the icon to include on the notification</param>
	void SendNotificationLocal(std::wstring notif, int slot);

	/// <summary>
	/// Converts a UTF-8 encoded std::string to a std::wstring.
	/// </summary>
	/// <param name="str">The UTF-8 encoded string to convert.</param>
	/// <returns>A std::wstring containing the converted wide characters from the input string.</returns>
	std::wstring to_wstring_n(const std::string& str);

	/// <summary>
	/// Determines whether the Brick Rigs game window is on the foreground
	/// </summary>
	/// <returns>Returns whether the Brick Rigs game window is on the foreground</returns>
	bool IsActiveWindow();

	/// <summary>
	/// Checks if Brick Rigs is currently in the main menu.
	/// </summary>
	/// <returns>true if Brick Rigs is in the main menu; otherwise, false.</returns>
	bool IsInMainMenu();

	/// <summary>
	/// Retrieves the bytecode of a specified function from a given class object.
	/// </summary>
	/// <param name="objectclass">Pointer to the UClass object representing the class containing the function.</param>
	/// <param name="classname">The name of the class as a string.</param>
	/// <param name="functionname">The name of the function whose bytecode is to be retrieved.</param>
	/// <returns>A vector of bytes containing the bytecode of the specified function.</returns>
	std::vector<uint8_t> GetFunctionBytecode(SDK::UClass* objectclass, std::string classname, std::string functionname);
}

SDK::ABrickCharacter* GetBrickCharacter();
SDK::ABrickPlayerController* GetBrickPlayerController();
SDK::ABrickPlayerState* GetBrickPlayerState();
SDK::ABrickGameMode* GetBrickGameMode();
SDK::ABrickGameState* GetBrickGameState();
SDK::ABrickGameSession* GetBrickGameSession();
SDK::AWorldSetupActor* GetWorldSetupActor();
SDK::UBrickAssetManager* GetBrickAssetManager();
SDK::ABrickPlayerController* GetBrickPlayerControllerFromName(std::string name);
SDK::ABrickPlayerController* GetBrickPlayerControllerFromID(std::string ID);
bool GetIsPlayerAdminFromName(std::string name);
bool GetIsPlayerHostFromName(std::string name);
PlayerInfo GetPlayerInfoFromController(SDK::ABrickPlayerController* controller);
std::string GetPlayerNameFromID(std::string ID);
std::string GetPlayerNameFromIDORName(std::string input);
inline SDK::ABrickPlayerController* GetBrickPlayerControllerFromIDORName(std::string input) { return GetBrickPlayerControllerFromName(GetPlayerNameFromIDORName(input)); }
std::string GetCurrentTimeFormatted();

inline bool isRelease()
{
	#ifndef _DEBUG
	return 1;
	#endif // !_DEBUG
	return 0;
}

#define Release(arbit) if(isRelease()) arbit
#define Debug(arbit) if(!isRelease()) arbit

SDK::TArray<SDK::AActor*>* AllActorsOfClass(SDK::TSubclassOf<SDK::AActor> sub);