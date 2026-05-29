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
#include "../../Include/Modules/o_client.h"
#include "BR-SDK.hpp"
#include <chrono>
#include <fstream>
#include <windows.h>
#include <shellapi.h>
#include <Hooking/Hook.hpp>

#pragma comment(lib, "shell32.lib")

void EnableHooks()
{
	AddChatMessageHook.Enable();
	BeginPlayHook.Enable();
	CleanUpWorldInternalHook.Enable();
}

void DestroyHooks()
{
	AddChatMessageHook.Destroy();
	BeginPlayHook.Destroy();
	CleanUpWorldInternalHook.Destroy();
}

static Hook<void(SDK::ABrickGameSession*, const SDK::FBrickChatMessage&)>
AddChatMessageHook(ABRICKGAMESESSION_ADDCHATMESSAGE, [](SDK::ABrickGameSession* This, const SDK::FBrickChatMessage& ChatMessage) -> void
{
	std::cout << "Someone said something" << std::endl;
	AddChatMessageHook.CallOriginalFunction(This, ChatMessage);
});

static Hook<void(SDK::UWorld*)>
BeginPlayHook(UWORLD_BEGINPLAY, [](SDK::UWorld* This) -> void
{
	BeginPlayHook.CallOriginalFunction(This);
	o_client::get()->connect();
});

static Hook<void(SDK::UWorld*, bool, bool, SDK::UWorld*)>
CleanUpWorldInternalHook(UWORLD_CLEANUPWORLDINTERNAL, [](SDK::UWorld* This, bool bSessionEnded, bool bCleanupResources, SDK::UWorld* NewWorld) -> void
{
	o_client::get()->disconnect();
	CleanUpWorldInternalHook.CallOriginalFunction(This, bSessionEnded, bCleanupResources, NewWorld);
});