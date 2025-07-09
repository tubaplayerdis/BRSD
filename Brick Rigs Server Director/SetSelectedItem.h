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

inline bool isParent(SDK::UPropertyContainerWidget* parent, SDK::UPropertyWidget* child)
{
    return parent->PropertyWidget == child;
}

namespace hooks
{
    class SetSelectedItem;
    inline SetSelectedItem* S_SetSelectedItem = nullptr; //Non-Inline causes link 2005

    //Hook should only be enabled when the CustomSettingsMenu is present.
    class SetSelectedItem : public Hook<void, SDK::UBoolPropertyWidget*, int, SDK::EValueChangedEventType>
    {
    public:

        static void __fastcall HookedFunction(SDK::UBoolPropertyWidget* This, int item, SDK::EValueChangedEventType EventType)
        {
            S_SetSelectedItem->OriginalFunction(This, item, EventType);
            using namespace psettings::elements;
            if (isParent(ChatCommandsPC, This))
            {
                std::cout << "toggled chat commands from settings menu at index:" << item << std::endl;
                //toggle on off chat commands through a module
            }
            
        }

        SetSelectedItem() : Hook(HOnItemSelected, HookedFunction) {}
    };
}