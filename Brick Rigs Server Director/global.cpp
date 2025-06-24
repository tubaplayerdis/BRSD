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

#include "global.h"
#include "SDK.hpp"
#include "windows.h"
#include <codecvt>
#include <locale>
#include <iostream>
#include <fstream>
#include "UNetDriver.h"

void global::pointers::InitPointers()
{
	Engine = SDK::UEngine::GetEngine();
	World = SDK::UWorld::GetWorld();
	Level = World->PersistentLevel;
	LocalController = World->OwningGameInstance->LocalPlayers[0]->PlayerController;
	mapLevelName = Level->Outer->GetName();
}

void global::pointers::UpdatePointers(SDK::UWorld* NewWorld)
{
	std::cout << "Updating Pointers!" << std::endl;
	updatingPointers = true;
	mapLevelName = "Changing";
	World = NewWorld;

	Level = World->PersistentLevel;
	mapLevelName = Level->Outer->GetName();
	std::cout << "New Map Name: " << mapLevelName << std::endl;
	LocalController = World->OwningGameInstance->LocalPlayers[0]->PlayerController;

	if (LocalController == nullptr || LocalController != World->OwningGameInstance->LocalPlayers[0]->PlayerController) {
		LocalController = World->OwningGameInstance->LocalPlayers[0]->PlayerController;
		std::cout << "Detected Change in PlayController! Switching." << std::endl;
	}

	updatingPointers = false;
}

SDK::ABrickCharacter* GetBrickCharacter()
{
	return static_cast<SDK::ABrickCharacter*>(global::LocalController->Character);
}

SDK::ABrickPlayerController* GetBrickPlayerController()
{
	return static_cast<SDK::ABrickPlayerController*>(global::LocalController);
}

SDK::ABrickPlayerState* GetBrickPlayerState()
{
	return static_cast<SDK::ABrickPlayerState*>(global::LocalController->PlayerState);
}

SDK::ABrickGameMode* GetBrickGameMode()
{
	return static_cast<SDK::ABrickGameMode*>(World()->AuthorityGameMode);
}

SDK::ABrickGameState* GetBrickGameState()
{
	return SDK::ABrickGameState::Get(World());
}

SDK::ABrickGameSession* GetBrickGameSession()
{
	return SDK::ABrickGameSession::Get(World());
}

SDK::AWorldSetupActor* GetWorldSetupActor()
{
	return SDK::AWorldSetupActor::Get(World());
}

SDK::UBrickAssetManager* GetBrickAssetManager()
{
	return SDK::UBrickAssetManager::Get();
}

SDK::ABrickPlayerController* GetBrickPlayerControllerFromName(std::string name)
{
	UC::TArray<SDK::AActor*> raw = UC::TArray<SDK::AActor*>();
	UC::TArray<SDK::AActor*>* what = &raw;
	SDK::UGameplayStatics::GetAllActorsOfClass(World(), SDK::ABrickPlayerController::StaticClass(), what);
	for (int i = 0; i < raw.Num(); i++)
	{
		SDK::ABrickPlayerController* cast = static_cast<SDK::ABrickPlayerController*>(raw[i]);
		SDK::ABrickPlayerState* state = static_cast<SDK::ABrickPlayerState*>(cast->PlayerState);
		if (state->GetPlayerNameText().ToString() == name) return cast;
	}
	return nullptr;
}

SDK::ABrickPlayerController* GetBrickPlayerControllerFromID(std::string ID)
{
	int playerID = -1;
	try {
		playerID = std::stoi(ID);
	}
	catch (...) {
		return nullptr;
	}
	UC::TArray<SDK::AActor*> raw = UC::TArray<SDK::AActor*>();
	UC::TArray<SDK::AActor*>* what = &raw;
	SDK::UGameplayStatics::GetAllActorsOfClass(World(), SDK::ABrickPlayerController::StaticClass(), what);
	for (int i = 0; i < raw.Num(); i++)
	{
		SDK::ABrickPlayerController* cast = static_cast<SDK::ABrickPlayerController*>(raw[i]);
		SDK::ABrickPlayerState* state = static_cast<SDK::ABrickPlayerState*>(cast->PlayerState);
		if (state->PlayerId == playerID) return cast;
	}
	return nullptr;
}

bool GetIsPlayerAdminFromName(std::string name)
{
	if (GetBrickPlayerControllerFromName(name) == nullptr) return false;
	return static_cast<SDK::ABrickPlayerState*>(GetBrickPlayerControllerFromName(name)->PlayerState)->IsAdmin();
}

bool GetIsPlayerHostFromName(std::string name)
{
	if (GetBrickPlayerControllerFromName(name) == nullptr) return false;
	return GetBrickPlayerControllerFromName(name) == global::LocalController;
}

PlayerInfo GetPlayerInfoFromController(SDK::ABrickPlayerController* controller)
{
	if (controller == nullptr) return PlayerInfo();
	PlayerInfo ret = PlayerInfo();
	SDK::ABrickPlayerState* state = static_cast<SDK::ABrickPlayerState*>(controller->PlayerState);
	ret.name = state->GetPlayerNameText().ToString();
	return ret;
}

std::string GetPlayerNameFromID(std::string ID)
{
	return GetPlayerInfoFromController(GetBrickPlayerControllerFromID(ID)).name;
}

std::string GetPlayerNameFromIDORName(std::string input)
{
	SDK::ABrickPlayerController* controller = nullptr;
	controller = GetBrickPlayerControllerFromName(input);
	if (!controller) controller = GetBrickPlayerControllerFromID(input);
	if (!controller) { return std::string("None"); }
	return GetPlayerInfoFromController(controller).name;
}

bool global::isMapValid()
{
	std::string name = mapLevelName;
	if (name == "City2") return true;
	else if (name == "Canyon") return true;
	else if (name == "City") return true;
	else if (name == "Desert") return true;
	else if (name == "GridMap") return true;
	else if (name == "Raceway") return true;
	else if (name == "Space") return true;
	else return false;
}

bool global::IsHost(SDK::UNetDriver* driver)
{
	if (pointers::updatingPointers || !UNetDriver::isServer(driver)) return false;
	return true;
}

void global::SendNotificationLocal(std::wstring notif, int slot)
{
	if (!isMapValid()) return;
	//WidgetSkib->SetColorAndOpacity(hooks::constructors::CreateLinearColor(0, 135, 255, 1)); //Change Color
	SDK::FName nameS = SDK::UKismetStringLibrary::Conv_StringToName(L"None");
	if (SDK::UGameOverlayWidget::Get(World) == nullptr) return;
	SDK::UHUDNotificationWidget* WidgetSkib = SDK::UGameOverlayWidget::Get(World)->CreateHUDNotification(nameS, true);
	WidgetSkib->TextBlock->SetText(SDK::UKismetTextLibrary::Conv_StringToText(notif.c_str()));
	WidgetSkib->TextBlock->SetTextStyle(SDK::EBrickUITextStyle::Default);
	WidgetSkib->TextBlock->SetStyleState(SDK::EBrickUIStyleState::Foreground);
	SDK::FBrickUIIconSlot slots = WidgetSkib->IconImage->IconSlot;
	slots.Index = slot;
	WidgetSkib->IconImage->SetIconSlot(slots);
	SDK::UGameOverlayWidget::Get(global::World)->AddHUDNotification(WidgetSkib, 0);
}

std::wstring global::to_wstring_n(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
	wstr.pop_back(); // remove null terminator
	return wstr;
}

bool global::IsActiveWindow()
{
	HWND hwnd = GetForegroundWindow();
	if (hwnd == NULL) return false;

	DWORD foregroundPid;
	if (GetWindowThreadProcessId(hwnd, &foregroundPid) == 0) return false;

	return (foregroundPid == GetCurrentProcessId());
}

uintptr_t GetModuleBaseN()
{
	return (uintptr_t)GetModuleHandle(NULL);
}

uintptr_t GetModuleBase(const wchar_t* moduleName)
{
	return (uintptr_t)GetModuleHandle(moduleName);
}

size_t GetModuleSizeN()
{
	MODULEINFO info = {};
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &info, sizeof(info));
	return info.SizeOfImage;
}

size_t GetModuleSize(const wchar_t* moduleName)
{
	MODULEINFO info = {};
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleW(moduleName), &info, sizeof(info));
	return info.SizeOfImage;
}

uintptr_t FindPattern(const char* pattern, const char* mask, uintptr_t base, size_t size)
{
	size_t patternLen = strlen(mask);

	for (size_t i = 0; i < size - patternLen; i++) {
		bool found = true;

		for (size_t j = 0; j < patternLen; j++) {
			if (mask[j] != '?' && pattern[j] != *(char*)(base + i + j)) {
				found = false;
				break;
			}
		}

		if (found)
			return base + i;
	}

	return 0;
}
