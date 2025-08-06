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
#include "offsets.h"
#include <windows.h>
#include <SDK.hpp>
#include "global.h"
#include "Hook.h"

namespace hooks
{
    class StartPlay;
    inline StartPlay* S_StartPlay = nullptr; //Non-Inline causes link 2005

    class StartPlay : public Hook<void(SDK::AGameMode*)>
    {
    public:

        static void __fastcall HookedFunction(SDK::AGameMode* This)
        {
            S_StartPlay->OriginalFunction(This);
            global::pointers::UpdatePointers(SDK::UWorld::GetWorld());
        }

        StartPlay() : Hook(StartPlay_S, StartPlay_M, HookedFunction) {}
    };
}