/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     OpenMenu.cpp		                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "OpenMenu.h"
#include "global.h"
#include "watermark.h"
#include <MinHook.h>

void __fastcall hooks::OpenMenu::HookedOpenMenuFunction(SDK::UMenuWidget* This, SDK::FName InMenu)
{
	OriginalOpenMenuFunction(This, InMenu);
	if (!This) return;
	if (InMenu.GetRawString().c_str() == nullptr) return;
	if (InMenu.GetRawString() == "InGameMenu") watermark::ShowWaterMark();
	if (InMenu.GetRawString() == "None" && global::isMapValid()) watermark::HideWaterWark();
}

bool hooks::OpenMenu::Init()
{
	if (initalized) return false;
	OpenMenuFunctionPointer = FindPattern(pattern, mask, GetModuleBaseN(), GetModuleSizeN());
	if (OpenMenuFunctionPointer == 0) return false;
	MH_STATUS ret = MH_CreateHook((LPVOID)OpenMenuFunctionPointer, &HookedOpenMenuFunction, (void**)&OriginalOpenMenuFunction);
	initalized = true;
	return ret == MH_OK;
}

void hooks::OpenMenu::Enable()
{
	if (!initalized || enabled) return;
	MH_EnableHook((LPVOID)OpenMenuFunctionPointer);
	enabled = true;
}

void hooks::OpenMenu::Disable()
{
	if (!initalized || !enabled) return;
	MH_DisableHook((LPVOID)OpenMenuFunctionPointer);
	enabled = false;
}
