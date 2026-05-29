/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     Hooks.h			                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once

#include <Hooking/Hook.hpp>
#include <BR-SDK.hpp>

extern Hook<void(SDK::ABrickGameSession*, const SDK::FBrickChatMessage&)> AddChatMessageHook;
extern Hook<void(SDK::UWorld*)> BeginPlayHook;
extern Hook<void(SDK::UWorld*, bool, bool, SDK::UWorld*)> CleanUpWorldInternalHook;

void EnableHooks();
void DestroyHooks();
