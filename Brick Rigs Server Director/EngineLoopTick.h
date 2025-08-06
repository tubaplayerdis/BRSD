/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     EngineLoopTick.h                                            */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    9 July 2025                                                 */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include "offsets.h"
#include <windows.h>
#include "Hook.h"
#include <queue>

inline std::queue<std::function<void()>> MainThreadQueue;

namespace hooks
{
    class EngineLoopTick;
    inline EngineLoopTick* S_EngineLoopTick = nullptr; //Non-Inline causes link 2005

    class EngineLoopTick : public Hook<void(void*, float)>
    {
    public:

        static void __fastcall HookedFunction(void* This, float time)
        {
            std::cout << "s" << std::endl;
            S_EngineLoopTick->OriginalFunction(This, time);
            while (!MainThreadQueue.empty())
            {
                auto fn = MainThreadQueue.front();
                MainThreadQueue.pop();
                fn();
            }
        }

        EngineLoopTick() : Hook(HEngineLoopTick, HookedFunction) {}
    };
}
