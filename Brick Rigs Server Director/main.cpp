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

		if (ContinuePress()) {
			std::cout << IsActiveWindow() << std::endl;
			continue;
		}

		if (UninjectPress() || doUninject) break;

		if (TogglePress()) {
			std::cout << UModHookSubsystem::GetModHookSubsystem()->ModHooks.Num() << std::endl;
			/*
			if (hooks::AddChatMessage::enabled) {
				hooks::AddChatMessage::Disable();
				SendNotificationLocal(L"Disabled Chat Commands!", 1); //Explore the icon atlas more
			}
			else {
				hooks::AddChatMessage::Enable();
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

	MessageAdmin("Uninjecting BRSD!");

	Notification(L"Uninjecting BRSD!", 9);
}
