/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     global.cpp		                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "../../Include/Global/Global.h"
#include <BR-SDK.hpp>
#include "windows.h"
#include <codecvt>
#include <locale>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "../../Include/Utils/PlayerInfo.h"

SDK::UWorld* GetWorld()
{
	return SDK::UWorld::GetWorld();
}

SDK::UEngine* GetEngine()
{
	return SDK::UEngine::GetEngine();
}

SDK::ABrickCharacter* GetBrickCharacter()
{
	return static_cast<SDK::ABrickCharacter*>(SDK::UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

SDK::ABrickPlayerController* GetBrickPlayerController()
{
	return static_cast<SDK::ABrickPlayerController*>(SDK::UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

SDK::ABrickPlayerState* GetBrickPlayerState()
{
	return static_cast<SDK::ABrickPlayerState*>(SDK::UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState);
}

SDK::ABrickGameMode* GetBrickGameMode()
{
	return static_cast<SDK::ABrickGameMode*>(GetWorld()->AuthorityGameMode);
}

SDK::ABrickGameState* GetBrickGameState()
{
	return SDK::ABrickGameState::Get(GetWorld());
}

SDK::ABrickGameSession* GetBrickGameSession()
{
	return SDK::ABrickGameSession::Get(GetWorld());
}

SDK::AWorldSetupActor* GetWorldSetupActor()
{
	return SDK::AWorldSetupActor::Get(GetWorld());
}

SDK::UBrickAssetManager* GetBrickAssetManager()
{
	return SDK::UBrickAssetManager::Get();
}

SDK::ABrickPlayerController* GetBrickPlayerControllerFromName(std::string name)
{
	UC::TArray<SDK::AActor*> raw = UC::TArray<SDK::AActor*>();
	UC::TArray<SDK::AActor*>* what = &raw;
	SDK::UGameplayStatics::GetAllActorsOfClass(GetWorld(), SDK::ABrickPlayerController::StaticClass(), what);
	for (int i = 0; i < raw.Num(); i++)
	{
		SDK::ABrickPlayerController* cast = static_cast<SDK::ABrickPlayerController*>(raw[i]);
		SDK::ABrickPlayerState* state = static_cast<SDK::ABrickPlayerState*>(cast->PlayerState);
		if (state->GetPlayerNameText().ToString() == name) return cast;
	}
	return nullptr;
}

SDK::ABrickPlayerController* GetBrickPlayerControllerFromID(std::string Id)
{
	return reinterpret_cast<SDK::ABrickPlayerController*>(SDK::UGameplayStatics::GetPlayerControllerFromID(GetWorld(), std::stoi(Id)));
}

bool GetIsPlayerAdminFromName(std::string name)
{
	if (GetBrickPlayerControllerFromName(name) == nullptr) return false;
	return static_cast<SDK::ABrickPlayerState*>(GetBrickPlayerControllerFromName(name)->PlayerState)->AdminRole == SDK::EAdminRole::Admin;
}

bool GetIsPlayerHostFromName(std::string name)
{
	if (GetBrickPlayerControllerFromName(name) == nullptr) return false;
	return GetBrickPlayerControllerFromName(name) == GetBrickPlayerController();
}

std::string GetPlayerNameFromID(std::string ID)
{
	return PlayerInfo::GetPlayerInfoFromController(GetBrickPlayerControllerFromID(ID)).name;
}

std::string GetPlayerNameFromIDORName(std::string input)
{
	SDK::ABrickPlayerController* controller = nullptr;
	controller = GetBrickPlayerControllerFromName(input);
	if (!controller) controller = GetBrickPlayerControllerFromID(input);
	if (!controller) { return std::string("None"); }
	return PlayerInfo::GetPlayerInfoFromController(controller).name;
}

std::string GetCurrentTimeFormatted()
{
	std::ostringstream oss;
	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	tm temi = tm(); localtime_s(&temi, &now_c);
	oss << "[" << std::put_time(&temi, "%F %T") << "] ";
	return oss.str();
}

SDK::TArray<SDK::AActor*>* AllActorsOfClass(SDK::TSubclassOf<SDK::AActor> sub)
{
	UC::TArray<SDK::AActor*> raw = UC::TArray<SDK::AActor*>();
	UC::TArray<SDK::AActor*>* what = &raw;
	SDK::UGameplayStatics::GetAllActorsOfClass(GetWorld(), sub, what);
	return what;
}

bool Statics::IsGameValid()
{
	std::string name = SDK::UGameplayStatics::GetCurrentLevelName(GetWorld(), true).ToString();
	if (name == "MainMenu" || name == "None" || name == "untitled_0") return false;
	return true;
}

bool Statics::IsHost()
{
	SDK::UKismetSystemLibrary::IsServer(GetWorld());
	return true;
}

std::wstring Statics::to_wstring_n(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
	wstr.pop_back(); // remove null terminator
	return wstr;
}

bool Statics::IsActiveWindow()
{
	HWND hwnd = GetForegroundWindow();
	if (hwnd == NULL) return false;

	DWORD foregroundPid;
	if (GetWindowThreadProcessId(hwnd, &foregroundPid) == 0) return false;

	return (foregroundPid == GetCurrentProcessId());
}

bool Statics::IsInMainMenu()
{
	return SDK::UGameplayStatics::GetCurrentLevelName(GetWorld(), true).ToString() == "MainMenu";
}
