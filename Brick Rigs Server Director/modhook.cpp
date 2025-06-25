/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     modhook.cpp		                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    25 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "modhook.h"
#include "global.h"
#include <MinHook.h>
#include "UModHookSubsystem.h"
#include "uibase.h"

bool modhook::CreateBRSDModHook()
{
    //hooked::Init();
    //hooked::Enable();
	return true;
}

void modhook::Uninitialize()
{
    //hooked::Disable();
}

bool modhook::OverrideMenu_Impl(SDK::UModHook* This, SDK::UMenuWidget* Widget, const SDK::FName& Context)
{
	std::cout << "omg!" << std::endl;
	return false;
}

void __fastcall modhook::hooked::HookedFunction(SDK::UModHook* This)
{
	std::cout << "Created mod hooks!" << std::endl;
	OriginalFunction(This);
	SDK::UModHook* ModHook = static_cast<SDK::UModHook*>(SDK::UGameplayStatics::SpawnObject(SDK::UModHook::StaticClass(), World()));
	constexpr size_t VTABLE_SIZE = 0x2F0;
	void** OriginalVTable = *(void***)ModHook;
	void** NewVTable = (void**)malloc(VTABLE_SIZE);
	memcpy(NewVTable, OriginalVTable, VTABLE_SIZE);

	// Offset 0x270 = index 0x270 / 8 = 0x4E = 78
	NewVTable[78] = (void*)&OverrideMenu_Impl;

	// Swap vtable
	*(void***)ModHook = NewVTable;

	ModHook->OverrideMenu(GetMenu(), NAME(L"Ohio"));

	UModHookSubsystem::GetModHookSubsystem()->ModHooks.Add(ModHook);
}

bool modhook::hooked::Init() {
	if (initalized) return false;
	FunctionPointer = (uintptr_t)GetModuleHandle(NULL) + 0x0D1A6E0;
	if (FunctionPointer == 0) return false;
	MH_STATUS ret = MH_CreateHook((LPVOID)FunctionPointer, &HookedFunction, (void**)&OriginalFunction);
	initalized = true;
	return ret == MH_OK;
}

void modhook::hooked::Enable()
{
	if (!initalized || enabled) return;
	MH_EnableHook((LPVOID)FunctionPointer);
	enabled = true;
}

void modhook::hooked::Disable()
{
	if (!initalized || !enabled) return;
	MH_DisableHook((LPVOID)FunctionPointer);
	enabled = false;
}
