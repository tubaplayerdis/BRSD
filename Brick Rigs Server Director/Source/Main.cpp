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
#include "../Include/Main.h"
#include "../Include/Hooks/Hooks.h"
#include "../Include/Global/Global.h"
#include "../Include/Modules/UI/Watermark.h"
#include "../Include/Modules/Command Engine/Messages.h"
#include "../Include/Modules/Server Director/Welcome.h"
#include "../Include/Modules/UI/UIBase.h"
#include "../Include/Hooks/General/AddChatMessage.h"
#include "../Include/Logging/logger.h"
#include "../Include/GUI/GuiMenus.h"
#include "../Include/FUnctions/loader.h"

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

void printHexValues(const std::string& str, int bytesPerLine = 16) {
	for (size_t i = 0; i < str.length(); ++i) {
		std::cout << std::hex << std::setw(2) << std::setfill('0')
			<< static_cast<int>(static_cast<unsigned char>(str[i]));

		if (i < str.length() - 1) {
			std::cout << " ";
		}

		if ((i + 1) % bytesPerLine == 0) {
			std::cout << std::endl;
		}
	}
	std::cout << std::dec << std::endl;
}

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

	menus::main_menu->display();

	if (IsHost()) welcome::SendWelcomeMessage();

	modules::InitalizeModules();

	std::cout << "Starting Main Loop!" << std::endl;

	while (true) {

		Sleep(10);

		if (ContinuePress()) continue;

		if (UninjectPress() || doUninject) break;

		if (TogglePress()) {
			//load_module();

			GuiManager* manager = GuiManager::get();
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

	GuiManager::shutdown();

	MessageHost("Uninjecting BRSD!");

	Notification(L"Uninjecting BRSD!", 9);

	DestroyLogging();
}
