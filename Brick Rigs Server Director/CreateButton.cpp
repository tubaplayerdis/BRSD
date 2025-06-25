/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     CreateButton.cpp                                            */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "CreateButton.h"
#include <MinHook.h>

SDK::UMenuButtonWidget* __fastcall hooks::CreateButton::HookedFunction(SDK::UMenuButtonPanelWidget* This, SDK::FText* DisplayText, int IconIndex, void* Delegate)
{
	std::cout << "Button Being Created: " << DisplayText->ToString() << std::endl;
	return OriginalFunction(This, DisplayText, IconIndex, Delegate);
}

bool hooks::CreateButton::Init() {
	if (initalized) return false;
	FunctionPointer = (uintptr_t)GetModuleHandle(NULL) + 0x0D56710;
	if (FunctionPointer == 0) return false;
	MH_STATUS ret = MH_CreateHook((LPVOID)FunctionPointer, &HookedFunction, (void**)&OriginalFunction);
	initalized = true;
	return ret == MH_OK;
}

void hooks::CreateButton::Enable()
{
	if (!initalized || enabled) return;
	MH_EnableHook((LPVOID)FunctionPointer);
	enabled = true;
}

void hooks::CreateButton::Disable()
{
	if (!initalized || !enabled) return;
	MH_DisableHook((LPVOID)FunctionPointer);
	enabled = false;
}
