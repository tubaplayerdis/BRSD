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
            Sleep(10);
            if (!This) return;
            std::cout << "Opening Menu: " << InMenu.GetRawString() << std::endl;
            if (InMenu.GetRawString().c_str() == nullptr) return;
            if (InMenu.GetRawString() == "InGameMenu") {
                watermark::ShowWaterMark();
                obutton::AddToButtonMenu();
            }
            if (InMenu.GetRawString() == "None" && global::isMapValid()) watermark::HideWaterWark();
        }

        OpenMenu() : Hook("\x48\x89\x54\x24\x10\x55\x53\x56\x57\x41\x54\x41\x55\x48\x8D\x6C\x24\xD1\x48\x81\xEC", "xxxxxxxxxxxxxxxxxxxx", HookedFunction) {}
    };
}