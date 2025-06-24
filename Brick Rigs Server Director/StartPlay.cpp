/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     StartPlay.cpp		                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "StartPlay.h"
#include "global.h"
#include <MinHook.h>

void __fastcall hooks::StartPlay::HookedStartPlayFunction(SDK::AGameMode* This)
{
	OriginalStartPlayFunction(This);
	std::cout << "Started Play!" << std::endl;
	global::pointers::UpdatePointers(SDK::UWorld::GetWorld());
}

bool hooks::StartPlay::Init()
{
	if (initalized) return false;
	StartPlayFunctionPointer = FindPattern(pattern, mask, GetModuleBaseN(), GetModuleSizeN());
	if (StartPlayFunctionPointer == 0) return false;
	MH_STATUS ret = MH_CreateHook((LPVOID)StartPlayFunctionPointer, &HookedStartPlayFunction, (void**)&OriginalStartPlayFunction);
	initalized = true;
	return ret == MH_OK;
}

void hooks::StartPlay::Enable()
{
	if (!initalized || enabled) return;
	MH_EnableHook((LPVOID)StartPlayFunctionPointer);
	enabled = true;
}

void hooks::StartPlay::Disable()
{
	if (!initalized || !enabled) return;
	MH_DisableHook((LPVOID)StartPlayFunctionPointer);
	enabled = false;
}
