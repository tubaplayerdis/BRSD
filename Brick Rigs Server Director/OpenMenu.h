/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     OpenMenu.h                                                  */
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
    namespace OpenMenu
    {
        inline bool enabled = false;
        inline bool initalized = false;
        inline const char* pattern = "\x48\x89\x54\x24\x10\x55\x53\x56\x57\x41\x54\x41\x55\x48\x8D\x6C\x24\xD1\x48\x81\xEC";
        inline const char* mask = "xxxxxxxxxxxxxxxxxxxx";
        inline uintptr_t OpenMenuFunctionPointer = 0;

        using OpenMenu_t = void(__fastcall*)(SDK::UMenuWidget* This, SDK::FName InMenu);
        inline OpenMenu_t OriginalOpenMenuFunction = nullptr;

        void __fastcall HookedOpenMenuFunction(SDK::UMenuWidget* This, SDK::FName InMenu);

        bool Init();
        void Enable();
        void Disable();
    }
}