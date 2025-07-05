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
#include "psettings.h"
#include "Hook.h"

namespace hooks
{
    class OnComboBoxMenuItemSelected;
    inline OnComboBoxMenuItemSelected* S_OnComboBoxMenuItemSelected = nullptr; //Non-Inline causes link 2005

    //Hook should only be enabled when the CustomSettingsMenu is present.
    class OnComboBoxMenuItemSelected : public Hook<void, SDK::UBrickComboBoxWidget*, int, SDK::EValueChangedEventType>
    {
    public:

        static void __fastcall HookedFunction(SDK::UBrickComboBoxWidget* This, int Item, SDK::EValueChangedEventType EventType)
        {
            S_OnComboBoxMenuItemSelected->OriginalFunction(This, Item, EventType);
            using namespace psettings::elements;
            if (IsComboBox(ChatCommandsPC, This)) {
                std::cout << "toggled chat commands from settings menu at index:" << Item << std::endl;
                //toggle on off chat commands through a module
            }
            
        }

        OnComboBoxMenuItemSelected() : Hook(HOnComboBoxMenuItemSelected, HookedFunction) {}
    };
}