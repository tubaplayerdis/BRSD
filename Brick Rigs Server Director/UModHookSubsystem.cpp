/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     UModHookSubsystem.cpp                                       */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    25 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "UModHookSubsystem.h"
#include "global.h"

SDK::UModHookSubsystem* UModHookSubsystem::GetModHookSubsystem()
{
	uintptr_t GetModHookSubsystemFunction = (uintptr_t)GetModuleHandle(NULL) + 0x0D20EE0;

	using GetModHookSubsystemFn = SDK::UModHookSubsystem*(__fastcall*)(const SDK::UObject* WorldContext);
	GetModHookSubsystemFn OnGetModHookSubsystemFunction = reinterpret_cast<GetModHookSubsystemFn>(GetModHookSubsystemFunction);

	return OnGetModHookSubsystemFunction(World());
}

void UModHookSubsystem::CreateModHooks(SDK::UModHookSubsystem* This)
{
	uintptr_t CreateModHooksFunction = (uintptr_t)GetModuleHandle(NULL) + 0x0D1A6E0;

	using CreateModHooksFn = void(__fastcall*)(SDK::UModHookSubsystem* This);
	CreateModHooksFn OnCreateModHooksFunction = reinterpret_cast<CreateModHooksFn>(CreateModHooksFunction);

	OnCreateModHooksFunction(This);
}
