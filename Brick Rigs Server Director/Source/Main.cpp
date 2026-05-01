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
#include "../Include/Modules/Messages.h"
#include "../Include/Hooks/General/AddChatMessage.h"
#include "../Include/Logging/logger.h"
#include "../Include/Modules/o_client.h"

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

	auto client = o_client::get();

	std::cout << "Starting Main Loop!" << std::endl;

	while (true)
	{

		Sleep(10);

		if (ContinuePress()) continue;

		if (UninjectPress() || doUninject) break;

	}


	hooks::DestroyHookObjects();

	MessageHost("Uninjecting BRSD!");

	Notification(L"Uninjecting BRSD!", 9);

	DestroyLogging();
}
