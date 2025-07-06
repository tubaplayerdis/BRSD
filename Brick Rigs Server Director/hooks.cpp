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

#pragma comment(lib, "shell32.lib")

bool hooks::InitHooks()
{
	std::cout << "Finding Hooks!" << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	S_AddChatMessage = new AddChatMessage();
	S_BeginPlay = new BeginPlay();
	S_OpenMenu = new OpenMenu();
	S_OnPlayerJoined = new OnPlayerJoined();
	S_StartPlay = new StartPlay();
	S_LoadMap = new LoadMap();
	S_OnClicked = new OnClicked();
	S_OnComboBoxMenuItemSelected = new OnComboBoxMenuItemSelected();
	bool ACMHook = S_AddChatMessage->IsInitialized(S_AddChatMessage);
	bool BPHook = S_BeginPlay->IsInitialized(S_BeginPlay);
	bool OMHook = S_OpenMenu->IsInitialized(S_OpenMenu);
	bool ONJHook = S_OnPlayerJoined->IsInitialized(S_OnPlayerJoined);
	bool RGHook = S_StartPlay->IsInitialized(S_StartPlay);
	bool LMHook = S_LoadMap->IsInitialized(S_LoadMap);
	bool OCHook = S_OnClicked->IsInitialized(S_OnClicked);
	bool SVHook = S_OnComboBoxMenuItemSelected->IsInitialized(S_OnComboBoxMenuItemSelected);
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
}

void hooks::DestroyHookObjects()
{
	//Deletes
	delete S_AddChatMessage;
	delete S_BeginPlay;
	delete S_OpenMenu;
	delete S_OnPlayerJoined;
	delete S_StartPlay;
	delete S_LoadMap;
	delete S_OnClicked;
	delete S_OnComboBoxMenuItemSelected;

	//Set nullptr
	S_AddChatMessage = nullptr;
	S_BeginPlay = nullptr;
	S_OpenMenu = nullptr;
	S_OnPlayerJoined = nullptr;
	S_StartPlay = nullptr;
	S_LoadMap = nullptr;
	S_OnClicked = nullptr;
	S_OnComboBoxMenuItemSelected = nullptr;
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