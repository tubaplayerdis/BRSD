/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     Global.h                                                    */
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
#include <BR-SDK.hpp>

#define STRING(string) UC::FString(string)
#define NAME(text) SDK::UKismetStringLibrary::Conv_StringToName(SDK::FString(text))
#define GetBoolString(text) (text ? "true" : "false")
#define BRSD L"BRSD"
#define Notification(message, icon) global::SendNotificationLocal(message, icon)
#define Verify(cls) cls::StaticClass()
#define WIN32_LEAN_AND_MEAN

/// <summary>
/// Runs code if in Release version
/// </summary>
/// <param name="arbit">Arbitrary code to run</param>
#define Release(arbit) if(isRelease()) arbit

/// <summary>
/// Runs code if in Debug version
/// </summary>
/// <param name="arbit">Arbitrary code to run</param>
#define Debug(arbit) if(!isRelease()) arbit

#undef TEXT
#define TEXT(text) SDK::UKismetTextLibrary::Conv_StringToText(SDK::FString(text))

//For "global" items, but ones that do not pollute the actual global namespace
namespace Statics
{

	/// <summary>
	/// Checks whether the map is playable.
	/// </summary>
	/// <returns>true if the map is playable; otherwise, false.</returns>
	bool IsGameValid();

	/// <summary>
	/// Determines whether the specified network driver is operating as the host.
	/// </summary>
	/// <returns>true if the driver is acting as the host; otherwise, false.</returns>
	bool IsHost();
	

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
}

SDK::UWorld* GetWorld();
SDK::UEngine* GetEngine();
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

SDK::TArray<SDK::AActor*>* AllActorsOfClass(SDK::TSubclassOf<SDK::AActor> sub);