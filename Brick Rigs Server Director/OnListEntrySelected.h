/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     OnListEntrySelected.h                                       */
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
#include "messages.h"
#include "global.h"
#include "Hook.h"
#include "Module.h"

//The idea is that this hook will create a button that will allow a Creation to added to the baclist from the browser
//Maybe switch to UpdateButtons()
namespace hooks
{
    class OnListEntrySelected;
    inline OnListEntrySelected* S_OnListEntrySelected = nullptr; //Non-Inline causes link 2005

    //Hook should only be enabled when the CustomSettingsMenu is present.
    class OnListEntrySelected : public Hook<void, SDK::UUGCBrowserWidget*, SDK::UPagedListEntryWidget*, bool>
    {
    public:

        static void __fastcall HookedFunction(SDK::UUGCBrowserWidget* This, SDK::UPagedListEntryWidget* Widget, bool bDoubleClick)
        {
            S_OnListEntrySelected->OriginalFunction(This, Widget, bDoubleClick);//Call the original function

            static_cast<SDK::UWBP_UGCBrowser_C*>(This)->CreateNewItemButton();
        }

        OnListEntrySelected() : Hook(HOnListEntrySelected, HookedFunction) {}
    };
}