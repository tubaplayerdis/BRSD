/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     StartPlay.h                                                 */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <windows.h>
#include <SDK.hpp>
#include "global.h"
#include "Hook.h"

namespace hooks
{
    class StartPlay;
    inline StartPlay* S_StartPlay = nullptr; //Non-Inline causes link 2005

    class StartPlay : public Hook<void, SDK::AGameMode*>
    {
    public:

        static void __fastcall HookedFunction(SDK::AGameMode* This)
        {
            S_StartPlay->OriginalFunction(This);
            std::cout << "Started Play!" << std::endl;
            global::pointers::UpdatePointers(SDK::UWorld::GetWorld());
        }

        StartPlay() : Hook("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x8B\x89\x80\x02\x00\x00\x48\x8B\x01", "xxxxxxxxxxxxxxxxxxx", HookedFunction) {}
    };
}