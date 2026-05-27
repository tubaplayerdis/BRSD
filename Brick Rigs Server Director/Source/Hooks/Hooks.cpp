/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     hooks.cpp			                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "../../Include/Hooks/Hooks.h"
#include "../../Include/Utils/Offsets.h"
#include "BR-SDK.hpp"
#include <chrono>
#include <fstream>
#include <windows.h>
#include <shellapi.h>
#include <Hooking/Hook.hpp>

#pragma comment(lib, "shell32.lib")

namespace
{
	Hook<void(SDK::ABrickGameSession*, const SDK::FBrickChatMessage&)>
	ACMH(ABRICKGAMESESSION_ADDCHATMESSAGE, [](SDK::ABrickGameSession* This, const SDK::FBrickChatMessage& ChatMessage) -> void
	{
		std::cout << "Someone said something" << std::endl;
		ACMH.CallOriginalFunction(This, ChatMessage);
	});
}



bool hooks::InitHooks()
{
	std::cout << "Finding Hooks!" << std::endl;
	ACMH.Create();
	return true;
}

// Combo box hook is Enabled based off the when the custom settings menu is enabled.
void hooks::EnableHooks()
{
	ACMH.Enable();
}

void hooks::DestroyHookObjects()
{
	ACMH.Destroy();
}

void hooks::OpenCrashFile()
{
	DWORD bufsize = GetCurrentDirectory(0, NULL);
	std::wstring curDir(bufsize, L'\0');
	GetCurrentDirectory(bufsize, &curDir[0]);

	// Remove trailing null char left by GetCurrentDirectory
	curDir.resize(wcslen(curDir.c_str()));

	// Add backslash if missing
	if (!curDir.empty() && curDir.back() != L'\\') {
		curDir += L'\\';
	}
	curDir += L"BRSDCRASH.txt";
	ShellExecute(NULL, L"open", curDir.c_str(), NULL, NULL, SW_SHOW);
}