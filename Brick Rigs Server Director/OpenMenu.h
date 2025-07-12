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
#include "offsets.h"
#include <windows.h>
#include <SDK.hpp>
#include "watermark.h"
#include "obutton.h"
#include "Hook.h"

namespace hooks
{
    class OpenMenu;
    inline OpenMenu* S_OpenMenu = nullptr; //Non-Inline causes link 2005

    class OpenMenu : public Hook<void, SDK::UMenuWidget*, SDK::FName>
    {
    public:

        static void __fastcall HookedFunction(SDK::UMenuWidget* This, SDK::FName InMenu)
        {
            S_OpenMenu->OriginalFunction(This, InMenu);
            obutton::CurrentButtonRef = nullptr;//gets set my AddToButtonMenu. if this is not set to nullptr it will cause odd behavior
            Sleep(10);
            if (!This) return;
            if (InMenu.GetRawString().c_str() == nullptr) return;
            if (InMenu.GetRawString() == "InGameMenu") {
                watermark::ShowWaterMark();
                obutton::AddToButtonMenu();
            }
            if (InMenu.GetRawString() == "None" && global::isMapValid()) watermark::HideWaterWark();
        }

        OpenMenu() : Hook(OpenMenu_S, OpenMenu_M, HookedFunction) {}
    };
}