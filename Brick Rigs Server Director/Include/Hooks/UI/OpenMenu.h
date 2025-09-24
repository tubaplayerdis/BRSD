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
#include "../../Utils/offsets.h"
#include <windows.h>
#include <BR-SDK.hpp>

namespace hooks
{
    class OpenMenu;
    inline OpenMenu* S_OpenMenu = nullptr; //Non-Inline causes link 2005

    class OpenMenu : public Hook<void(SDK::UMenuWidget*, SDK::FName)>
    {
    public:

        static void __fastcall HookedFunction(SDK::UMenuWidget* This, SDK::FName InMenu)
        {
            S_OpenMenu->OriginalFunction(This, InMenu);
            Sleep(10);
            if (!This) return;
            if (InMenu.GetRawString().c_str() == nullptr) return;
            if (InMenu.GetRawString() == "InGameMenu") {
                watermark::ShowWaterMark();
            }
            else if (InMenu.GetRawString() == "Players") {
            }
            if (InMenu.GetRawString() == "None" && global::isMapValid()) watermark::HideWaterWark();
        }

        OpenMenu() : Hook(UMENUWIDGET_OPENMENU, HookedFunction) {}
    };
}