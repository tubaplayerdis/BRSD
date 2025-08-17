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

#include "hooks.h"
#include <chrono>
#include <fstream>
#include <windows.h>
#include <shellapi.h>
#include "Elevator.h"

#pragma comment(lib, "shell32.lib")

//Backwards compatibility macros
#define _IsInitialized(ptr) ptr->IsInitialized();
#define InitializeHook(ptr, type) \
ptr = new type(); \
ptr->Create(); \

#define DestroyHook(ptr) \
ptr->Disable(); \
delete ptr; \
ptr = nullptr; \

bool hooks::InitHooks()
{
	std::cout << "Finding Hooks!" << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	InitializeHook(S_AddChatMessage, AddChatMessage);
	InitializeHook(S_BeginPlay, BeginPlay);
	InitializeHook(S_OpenMenu, OpenMenu);
	InitializeHook(S_OnPlayerJoined, OnPlayerJoined);
	InitializeHook(S_StartPlay, StartPlay);
	InitializeHook(S_LoadMap, LoadMap);
	InitializeHook(S_OnClicked, OnClicked);
	InitializeHook(S_SetSelectedItem, SetSelectedItem);
	InitializeHook(S_RestartPlayerAt, RestartPlayerAt);
	InitializeHook(S_UpdateButtons, UpdateButtons);
	bool ACMHook = _IsInitialized(S_AddChatMessage);
	bool BPHook = _IsInitialized(S_BeginPlay);
	bool OMHook = _IsInitialized(S_OpenMenu);
	bool ONJHook = _IsInitialized(S_OnPlayerJoined);
	bool RGHook = _IsInitialized(S_StartPlay);
	bool LMHook = _IsInitialized(S_LoadMap);
	bool OCHook = _IsInitialized(S_OnClicked);
	bool SVHook = _IsInitialized(S_SetSelectedItem);
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Elapsed Time Finding Hooks: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
	if (!ACMHook || !BPHook || !OMHook || !ONJHook || !RGHook || !LMHook || !OCHook || !SVHook) {
		//print cases
		std::cout << "ACMHOOK: " << ACMHook << std::endl;
		std::cout << "BPHOOK: " << BPHook << std::endl;
		std::cout << "OMHOOK: " << OMHook << std::endl;
		std::cout << "ONJHOOK: " << ONJHook << std::endl;
		std::cout << "RGHOOK: " << RGHook << std::endl;
		std::cout << "LMHook: " << LMHook << std::endl;
		std::cout << "CBHook: " << OCHook << std::endl;
		std::cout << "SVHOOK: " << SVHook << std::endl;

		//Print cases to file
		std::ofstream saveFile;
		saveFile.open("BRSDCRASH.txt", std::ios::trunc);
		if (!saveFile.bad() && !saveFile.fail() && saveFile.is_open()) {
			saveFile << "Hooks have a possibility of loading incorrectly given a version change for Brick Rigs (An Update). This is normal and an update should be out soon fixing this if that is the case." << std::endl;
			saveFile << "ACMHOOK: " << ACMHook << std::endl;
			saveFile << "BPHOOK: " << BPHook << std::endl;
			saveFile << "OMHOOK: " << OMHook << std::endl;
			saveFile << "ONJHOOK: " << ONJHook << std::endl;
			saveFile << "RGHOOK: " << RGHook << std::endl;
			saveFile << "LMHOOK: " << LMHook << std::endl;
			saveFile << "OCHOOK: " << OCHook << std::endl;
			saveFile << "SVHOOK: " << SVHook << std::endl;
			saveFile.close();
		}
		return false;
	}
	return true;
}

// Combo box hook is Enabled based off the when the custom settings menu is enabled.
void hooks::EnableHooks()
{
	S_AddChatMessage->Enable();
	S_BeginPlay->Enable();
	S_OpenMenu->Enable();
	S_OnPlayerJoined->Enable();
	S_StartPlay->Enable();
	S_LoadMap->Enable();
	S_OnClicked->Enable();
	S_SetSelectedItem->Enable();
	S_RestartPlayerAt->Enable();
	S_UpdateButtons->Enable();
	//S_EngineLoopTick->Enable();
}

void hooks::DestroyHookObjects()
{
	DestroyHook(S_AddChatMessage);
	DestroyHook(S_BeginPlay);
	DestroyHook(S_OpenMenu);
	DestroyHook(S_OnPlayerJoined);
	DestroyHook(S_StartPlay);
	DestroyHook(S_LoadMap);
	DestroyHook(S_OnClicked);
	DestroyHook(S_SetSelectedItem);
	DestroyHook(S_RestartPlayerAt);
	DestroyHook(S_UpdateButtons);
	Elevator::Get()->DestroyHooks();
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