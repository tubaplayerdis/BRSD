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
#include <sstream>
#include <chrono>
#include "Function.h"

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
	updatingPointers = true;
	mapLevelName = "Changing";
	World = NewWorld;

	Level = World->PersistentLevel;
	mapLevelName = Level->Outer->GetName();
	LocalController = World->OwningGameInstance->LocalPlayers[0]->PlayerController;

	if (LocalController == nullptr || LocalController != World->OwningGameInstance->LocalPlayers[0]->PlayerController) {
		LocalController = World->OwningGameInstance->LocalPlayers[0]->PlayerController;
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
	SDK::UGameplayStatics::GetAllActorsOfClass(World(), sub, what);
	return what;
}

bool global::isMapValid()
{
	std::string name = SDK::UGameplayStatics::GetCurrentLevelName(World(), true).ToString();
	std::cout << "Current Level Name: " << name << std::endl;
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
	if (!driver) return false;
	using IsServerFn = bool(__fastcall*)(SDK::UNetDriver*);
	void** vtable = *(void***)driver;
	IsServerFn IsServerFunc = reinterpret_cast<IsServerFn>(vtable[0x378 / 8]);
	bool isServer = IsServerFunc(driver);
	if (pointers::updatingPointers || !isServer) return false;
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

bool global::IsInMainMenu()
{
	return SDK::UGameplayStatics::GetCurrentLevelName(World(), true).ToString() == "MainMenu";
}

std::vector<uint8_t> global::GetFunctionBytecode(SDK::UClass* objectclass,std::string classname, std::string functionname)
{
	std::vector<uint8_t> ret = std::vector<uint8_t>();
	SDK::UFunction* Fn = objectclass->GetFunction(classname, functionname);
	if (Fn) std::cout << "Function Flags: " << Fn->FunctionFlags << std::endl;
	if (!Fn || Fn->Script.Num() == 0) {
		if (!Fn) std::cout << "Function was not found!" << std::endl;
		std::cout << "Script was not found!" << std::endl;
		return ret;
	}
	std::cout << "Bytecode for" << Fn->GetName() << std::endl;
	for (int i = 0; i < Fn->Script.Num(); ++i)
	{
		std::string s = std::format("{:02X} ", Fn->Script[i]);
		std::cout << s << std::endl;
		ret.push_back(Fn->Script[i]);
	}
	std::cout << std::endl;
	return ret;
}

SDK::UClass* global::classes::StaticLoadClass(SDK::UClass* BaseClass, SDK::UObject* InOuter, const wchar_t* InName, const wchar_t* Filename, unsigned int LoadFlags, SDK::UPackageMap* Sandbox)
{
	if (addy == 0)
	{
		std::cout << "Calculating first time class loading" << std::endl;
		addy = FindPatternF(signature, mask);
	}

	if (addy == 0) { std::cout << "Failed to find StaticLoadClass signatue!" << std::endl; return nullptr; }

	return CallGameFunctionO<SDK::UClass*, SDK::UClass*, SDK::UObject*, const wchar_t*, const wchar_t*, unsigned int, SDK::UPackageMap*>(addy, BaseClass, InOuter, InName, Filename, LoadFlags, Sandbox);
}

bool global::classes::IsClassLoaded(SDK::UClass* classptr)
{
	if (!classptr) return false;
	if (!classptr->DefaultObject) return false;
	return true;
}

void global::packages::FullyLoadPackage(SDK::UPackage* This)
{
	if (addy == 0)
	{
		std::cout << "Calculating first time load package" << std::endl;
		addy = FindPatternF(signature, mask);
	}

	if (addy == 0) { std::cout << "Failed to find FullyLoadPackage signatue!" << std::endl; return; }

	CallGameFunctionO<void, SDK::UPackage*>(addy, This);
}
