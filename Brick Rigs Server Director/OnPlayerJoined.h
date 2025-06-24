/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     OnPlayerJoined.h                                            */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <windows.h>
#include <SDK.hpp>

namespace hooks
{
    namespace OnPlayerJoined
    {
        inline bool enabled = false;
        inline bool initalized = false;
        inline const char* pattern = "\x48\x83\xEC\x48\x48\x89\x5C\x24\x58\x33\xDB\x48\x89\x74\x24\x68\x48\x89\x7C\x24\x40";
        inline const char* mask = "xxxxxxxxxxxxxxxxxxxx";
        inline uintptr_t OnPlayerJoinedFunctionPointer = 0;

        using OnPlayerJoined_t = void(__fastcall*)(SDK::ABrickGameSession* This, SDK::ABrickPlayerController* PC);
        inline OnPlayerJoined_t OriginalOnPlayerJoinedFunction = nullptr;

        void __fastcall HookedOnPlayerJoinedFunction(SDK::ABrickGameSession* This, SDK::ABrickPlayerController* PC);

        bool Init();
        void Enable();
        void Disable();
    }
}