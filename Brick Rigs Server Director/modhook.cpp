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
	//checks::ForEachModHookSingleton = new checks::ForEachModHook();
	//checks::ForEachModHookSingleton->Enable();
	//checks::OverrideMenuHookSingleton = new checks::OverrideMenuHook();
	//checks::OverrideMenuHookSingleton->Enable();
	return true;
}

void modhook::Uninitialize()
{
    //hooked::Disable();
	//delete checks::ForEachModHookSingleton;
	//checks::ForEachModHookSingleton = nullptr;
	//delete checks::OverrideMenuHookSingleton;
	//checks::OverrideMenuHookSingleton = nullptr;
}

bool modhook::OverrideMenu_Impl(SDK::UModHook* This, SDK::UMenuWidget* Widget, const SDK::FName& Context)
{
	std::cout << "omg!" << std::endl;
	return false;
}

void __fastcall modhook::hooked::HookedFunction(SDK::UModHook* This)
{
	OriginalFunction(This);
	
	SDK::UModHook* ModHook = static_cast<SDK::UModHook*>(SDK::UGameplayStatics::SpawnObject(SDK::UModHook::StaticClass(), World()));
	/*
	constexpr size_t VTABLE_SIZE = 0x2F0;
	void** OriginalVTable = *(void***)ModHook;
	void** NewVTable = (void**)malloc(VTABLE_SIZE);
	memcpy(NewVTable, OriginalVTable, VTABLE_SIZE);

	// Offset 0x270 = index 0x270 / 8 = 0x4E = 78
	NewVTable[78] = (void*)&OverrideMenu_Impl;

	// Swap vtable
	*(void***)ModHook = NewVTable;

	*/

	#define FUNC_Public      0x00000001
	#define FUNC_Native      0x00000400
	#define FUNC_BlueprintCallable 0x00001000

	//ModHook->Class->GetFunction("ModHook", "OverrideMenu")->FunctionFlags |= FUNC_Public | FUNC_Native | FUNC_BlueprintCallable;

	SDK::TAllocatedArray<SDK::UModHook*> Temp = SDK::TAllocatedArray<SDK::UModHook*>(UModHookSubsystem::GetModHookSubsystem()->ModHooks.Max()+1);
	for (int i = 0; i < UModHookSubsystem::GetModHookSubsystem()->ModHooks.Num(); i++)
	{
		Temp.Add(UModHookSubsystem::GetModHookSubsystem()->ModHooks[i]);
	}
	Temp.Add(ModHook);

	UC::TArray<SDK::UModHook*> Swap = Temp.operator UC::TArray<SDK::UModHook*>();

	std::cout << "Copied!" << std::endl;

	UModHookSubsystem::GetModHookSubsystem()->ModHooks = Swap;

	std::cout << "Swapped!" << std::endl;
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
