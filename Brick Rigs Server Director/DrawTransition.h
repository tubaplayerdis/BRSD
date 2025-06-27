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

        DrawTransition() : Hook("\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x40\x80\xB9\x88\x00\x00\x00\x00\x48\x8B\xF2\x48\x8B\xF9\x0F\x85\x00\x00\x00\x00\x8B\x41\x08\x45\x33\xC0\xC1\xE8\x04\xA8\x01\x41\x8B\xC0", "xxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxx", HookedFunction, false) {}
    };
}
