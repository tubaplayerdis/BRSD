/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     UpdateButtons.h                                             */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 July 2025                                                */
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
#include "Blacklist.h"
#include "functions.h"

#define ADD_ICON_INDEX 1
#define ADD_VEHICLE_BUTTON_MESSAGE L"Blacklist"
#define REMOVE_VEHICLE_BUTTON_MESSAGE L"Un-Blacklist"

//The idea is that this hook will create a button that will allow a Creation to added to the baclist from the browser
//Maybe switch to UpdateButtons()

#define GetBlacklistButton() hooks::UpdateButtons::ActiveButtonRef

namespace hooks
{

    class UpdateButtons;
    inline UpdateButtons* S_UpdateButtons = nullptr; //Non-Inline causes link 2005

    //Hook should only be enabled when the CustomSettingsMenu is present.
    class UpdateButtons : public Hook<void, SDK::UUGCBrowserWidget*>
    {
    public:
        static inline SDK::UMenuButtonWidget* ActiveButtonRef = nullptr;
        static inline SDK::UUGCBrowserWidget* ActiveBrowserRef = nullptr;

        static void __fastcall HookedFunction(SDK::UUGCBrowserWidget* This)
        {
            S_UpdateButtons->OriginalFunction(This);//Call the original function

            ActiveBrowserRef = This;

            SDK::FUGCFileInfo* FileInfo = GetSelectedFileInfo(This);

            //Check if the file being access is a vehicle
            if (!FileInfo || FileInfo->FileType != SDK::EUGCFileType::Online) return;
            

            bool isAlreadyBanned = Blacklist::Get()->IsVehicleSteamLinkPositive(*FileInfo);

            SDK::FText ButtonText;
            
            if (isAlreadyBanned) ButtonText = TEXT(REMOVE_VEHICLE_BUTTON_MESSAGE);
            else ButtonText = TEXT(ADD_VEHICLE_BUTTON_MESSAGE);

            SDK::TDelegate<void()> Dele = This->GetButtonPanel()->Buttons[0]->OnClickedDelegate;
            Dele.BoundFunction.FunctionName = NAME(L"None");
            SDK::UMenuButtonWidget* menuButton = static_cast<SDK::UWBP_UGCBrowser_C*>(This)->GetButtonPanel()->CreateButton(ButtonText, ADD_ICON_INDEX, Dele);
            ActiveButtonRef = menuButton;

            if (isAlreadyBanned) menuButton->SetColorStyle(SDK::EBrickUIColorStyle::Negative);
            
        }

        UpdateButtons() : Hook(HUpdateButtons, HookedFunction) {}
    };
}