/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     LoadMap.h                                                   */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <Windows.h>
#include <SDK.hpp>
#include "global.h"
#include "Hook.h"

namespace hooks
{
    class LoadMap;
    inline LoadMap* S_LoadMap = nullptr; //Non-Inline causes link 2005

    class LoadMap : public Hook<char, SDK::UEngine*, SDK::FWorldContext*, void*, void*, void*>
    {
    public:

        static char __fastcall HookedFunction(SDK::UEngine* This, SDK::FWorldContext* WorldContext, void* URL, void* Pending, void* Error)
        {
            char ret = S_LoadMap->OriginalFunction(This, WorldContext, URL, Pending, Error);
            global::mapLevelName = World()->PersistentLevel->Outer->GetName();
            if (global::mapLevelName == "MainMenu") watermark::ShowWaterMark();
            Sleep(20);
            return ret;
        }

        LoadMap() : Hook("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x80\x48\x81\xEC\x80\x01\x00\x00\x0F\x29\xBC\x24\x60\x01\x00\x00\x48\x8B\x05\x65\x77\xA7\x01", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", HookedFunction) {}
    };
}