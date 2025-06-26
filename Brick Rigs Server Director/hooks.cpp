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
#include <MinHook.h>
#include "global.h"

#pragma comment(lib, "shell32.lib")

bool hooks::InitHooks()
{
	std::cout << "Finding Hooks!" << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	bool ACMHook = AddChatMessage::Init();
	bool BPHook = BeginPlay::Init();
	bool OMHook = OpenMenu::Init();
	bool ONJHook = OnPlayerJoined::Init();//0
	bool RGHook = StartPlay::Init();
	bool LMHook = LoadMap::Init();
	bool OCHook = OnClicked::Init();
	bool STHook = true;
	try {
		SetTitleTextSingleton = new SetTitleText();
	}
	catch (HookingException e) {
		std::cout << e.what() << std::endl;
		STHook = false;
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Elapsed Time Finding Hooks: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
	if (!ACMHook || !BPHook || !OMHook || !ONJHook || !RGHook || !LMHook || !OCHook || !STHook) {
		//print cases
		std::cout << "ACMHOOK: " << ACMHook << std::endl;
		std::cout << "BPHOOK: " << BPHook << std::endl;
		std::cout << "OMHOOK: " << OMHook << std::endl;
		std::cout << "ONJHOOK: " << ONJHook << std::endl;
		std::cout << "RGHOOK: " << RGHook << std::endl;
		std::cout << "LMHook: " << LMHook << std::endl;
		std::cout << "CBHook: " << OCHook << std::endl;
		std::cout << "STHOOK: " << STHook << std::endl;

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
			saveFile << "STHOOK: " << STHook << std::endl;
			saveFile.close();
		}
		return false;
	}
	return true;
}

void hooks::EnableHooks()
{
	AddChatMessage::Enable();
	BeginPlay::Enable();
	OpenMenu::Enable();
	OnPlayerJoined::Enable();
	StartPlay::Enable();
	LoadMap::Enable();
	OnClicked::Enable();
	SetTitleTextSingleton->Enable();
}

void hooks::DestroyHookObjects()
{
	delete SetTitleTextSingleton;
	SetTitleTextSingleton = nullptr;
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
