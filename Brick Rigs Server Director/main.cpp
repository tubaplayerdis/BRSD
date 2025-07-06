/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     main.cpp                                                    */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include <iostream>
#include <MinHook.h>
#include <windows.h>
#include "main.h"
#include "hooks.h"
#include "global.h"
#include "watermark.h"
#include "messages.h"
#include "welcome.h"
#include "obutton.h"
#include "psettings.h"
#include "uibase.h"
#include "functions.h"
#include "AddChatMessage.h"
#include "logger.h"
#include "spawnutils.h"

using namespace global;

#define PRESSED 0x8000

#ifdef _DEBUG
#define UninjectPress() GetAsyncKeyState(VK_DIVIDE) & 1
#define TogglePress() GetAsyncKeyState(VK_MULTIPLY) & 1
#else
#define UninjectPress() (IsActiveWindow() && (GetAsyncKeyState(VK_CONTROL) & PRESSED) && (GetAsyncKeyState('U') & PRESSED))
#define TogglePress() (IsActiveWindow() && (GetAsyncKeyState(VK_CONTROL) & PRESSED) && (GetAsyncKeyState('T') & PRESSED))
#endif

#define ContinuePress() (IsActiveWindow() && (GetAsyncKeyState(VK_RETURN) & 1))

void MainLoop()
{
	InitLogging();

	std::cout << reinterpret_cast<const char*>(CommandLineLogo) << std::endl;

	if (!hooks::InitHooks()) {
		if (MessageBox(GetActiveWindow(), L"Failed To Hook Critical Functions. Uninjecting BCSD. Would you like to look at the crash log?", L"Uninjecting BRSD", MB_YESNO) == IDYES) hooks::OpenCrashFile();
		return;
	}

	global::pointers::InitPointers();

	hooks::EnableHooks();

	watermark::InitalizeWaterMark();

	if(uibase::IsInGameMenuOpen()) obutton::AddToButtonMenu();

	if (IsHost()) welcome::SendWelcomeMessage();

	psettings::CreateCustomSettingsPage();

	std::cout << "Starting Main Loop!" << std::endl;

	while (true) {

		Sleep(10);

		if (ContinuePress()) continue;

		if (UninjectPress() || doUninject) break;

		if (TogglePress()) {
			SDK::TSoftClassPtr<SDK::UClass> ptr = SDK::TSoftClassPtr<SDK::UClass>();
			const SDK::FName path = SDK::UKismetStringLibrary::Conv_StringToName(SDK::FString(L"/Game/BrickRigs/UI/Properties/WBP_BoolProperty.WBP_BoolProperty_C"));
			_spawnutils::SetPath(&ptr.ObjectID, path);
			_spawnutils::RequestSyncLoad(ptr.ObjectID);
			/*
			if (hooks::S_AddChatMessage->IsEnabled(hooks::S_AddChatMessage)) {
				hooks::S_AddChatMessage->Disable();
				LOG("Disabled Chat Commands");
				SendNotificationLocal(L"Disabled Chat Commands!", 1); //Explore the icon atlas more
			}
			else {
				hooks::S_AddChatMessage->Enable();
				LOG("Enabled Chat Commands");
				SendNotificationLocal(L"Enabled Chat Commands!", 0);
			}
			*/
		}
	}
	
	watermark::HideWaterWark();
	watermark::UnInitalizeWaterMark();

	psettings::Uninitalize();

	hooks::DestroyHookObjects();

	uibase::Cleanup();

	delete _spawnutils::cb;

	MessageHost("Uninjecting BRSD!");

	Notification(L"Uninjecting BRSD!", 9);

	DestroyLogging();
}
