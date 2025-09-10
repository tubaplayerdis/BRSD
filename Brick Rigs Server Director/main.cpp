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
#include <windows.h>
#include "main.h"
#include "hooks.h"
#include "global.h"
#include "watermark.h"
#include "messages.h"
#include "welcome.h"
#include "uibase.h"
#include "functions.h"
#include "AddChatMessage.h"
#include "logger.h"
#include "EngineLoopTick.h"
#include "Elevator.h"
#include "menus.h"

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
		if (MessageBox(GetActiveWindow(), L"Failed To Hook Critical Functions. Uninjecting BRSD. Would you like to look at the crash log?", L"Uninjecting BRSD", MB_YESNO) == IDYES) hooks::OpenCrashFile();
		return;
	}

	global::pointers::InitPointers();

	hooks::EnableHooks();

	watermark::InitalizeWaterMark();

	menus::main_menu.display();

	if (IsHost()) welcome::SendWelcomeMessage();

	modules::InitalizeModules();

	std::cout << "Starting Main Loop!" << std::endl;

	while (true) {

		Sleep(10);

		if (ContinuePress()) continue;

		if (UninjectPress() || doUninject) break;

		if (TogglePress()) {
			gui_manager* manager = gui_manager::get();
			if (manager->are_all_hidden()) {
				manager->display_all_previous();
			}
			else {
				manager->hide_all();
			}
		}

	}
	
	watermark::HideWaterWark();
	watermark::UnInitalizeWaterMark();

	modules::DestroyModules();

	hooks::DestroyHookObjects();

	uibase::Cleanup();

	gui_manager::shutdown();

	MessageHost("Uninjecting BRSD!");

	Notification(L"Uninjecting BRSD!", 9);

	DestroyLogging();
}
