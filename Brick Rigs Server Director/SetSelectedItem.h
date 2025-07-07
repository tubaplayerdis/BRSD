/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     SetSelectedItem.h                                           */
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
    class SetSelectedItem;
    inline SetSelectedItem* S_SetSelectedItem = nullptr; //Non-Inline causes link 2005

    //Hook should only be enabled when the CustomSettingsMenu is present.
    class SetSelectedItem : public Hook<void, SDK::UBrickComboBoxWidget*, int>
    {
    public:

        static void __fastcall HookedFunction(SDK::UBrickComboBoxWidget* This, int Item)
        {
            std::cout << "combo box item selected" << std::endl;
            S_SetSelectedItem->OriginalFunction(This, Item);
            using namespace psettings::elements;
            if (IsComboBox(ChatCommandsPC, This)) {
                std::cout << "toggled chat commands from settings menu at index:" << Item << std::endl;
                //toggle on off chat commands through a module
            }
            
        }

        SetSelectedItem() : Hook(HSetSelectedItem, HookedFunction) {}
    };
}