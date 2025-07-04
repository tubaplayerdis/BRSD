/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     DrawTransition.h                                            */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <windows.h>
#include <SDK.hpp>
#include "functions.h"
#include "Hook.h"
#include "offsets.h"

namespace hooks
{
    class DrawTransition;
    inline DrawTransition* S_DrawTransition = nullptr; //Non-Inline causes link 2005

    class DrawTransition : public Hook<void, SDK::UGameViewportClient*, SDK::UCanvas*>
    {
    public:

        static void __fastcall HookedFunction(SDK::UGameViewportClient* This, SDK::UCanvas* Canvas)
        {
            SDK::FVector2D loc = SDK::FVector2D();
            loc.X = 100;
            loc.Y = 100;
            SDK::FVector2D size = SDK::FVector2D();
            size.X = 100;
            size.Y = 100;
            Canvas->K2_DrawBox(loc, size, 10, FLinearColor::CreateLinearColor(255, 0, 0, 1));
            S_DrawTransition->OriginalFunction(This, Canvas);
        }

        DrawTransition() : Hook(DrawTransition_S, DrawTransition_M, HookedFunction, SAFE) {}
    };
}
