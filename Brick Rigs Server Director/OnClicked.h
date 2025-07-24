/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     OnClicked.h		                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    25 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include "offsets.h"
#include "SDK.hpp"
#include "windows.h"
#include "Hook.h"
#include "global.h"
#include "obutton.h"
#include "uibase.h"
#include "psettings.h"
#include "Function.h"
#include "UpdateButtons.h"

namespace UMenuWidget
{
	inline void OpenMenu(SDK::FName InMenu)
	{
		return CallGameFunction<void, SDK::UMenuWidget*, SDK::FName>(FOpenMenu, GetMenu(), InMenu);
	}
}

namespace hooks
{
	class OnClicked;
	inline OnClicked* S_OnClicked = nullptr; //Non-Inline causes link 2005

	class OnClicked : public Hook<void, SDK::UMenuButtonWidget*>
	{
	public:

		static void __fastcall HookedFunction(SDK::UMenuButtonWidget* This)
		{
			//Resetting the swapped out page
			if (GetMenu() && GetMenu()->CurrentMenuPage && GetMenu()->CurrentMenuPage == psettings::MockPage) {
				//psettings::SetHook(false);
				static_cast<SDK::UBrickBorder*>(psettings::CustomSettingsPage->Slot->Parent)->SetContent(psettings::MockPage);
				S_OnClicked->OriginalFunction(This);
				return;
			}

			if (This == obutton::CurrentButtonRef) {
				if (GetMenu()->CurrentMenuPage) GetMenu()->RemoveMenuPage(GetMenu()->CurrentMenuPage);
				const SDK::FText title = TEXT(L"BRSD");
				UMenuWidget::OpenMenu(NAME(L"InGameMenu/BSRD"));
				GetMenu()->AddMenuPage(psettings::MockPage);
				GetMenu()->CurrentMenuPage = psettings::MockPage;
				SDK::UBorderSlot* slot = static_cast<SDK::UBorderSlot*>(psettings::MockPage->Slot);
				if (!slot || !slot->Parent)
				{
					return; //silently fail ig.
				}
				SDK::UBrickBorder* Parent = static_cast<SDK::UBrickBorder*>(slot->Parent);
				Parent->SetContent(psettings::CustomSettingsPage);
				psettings::PrepareCustomSettingsPage();
				psettings::SetVisibility(SDK::ESlateVisibility::Visible);
				//psettings::SetHook(true);
			}
			else if (This == GetBlacklistButton())
			{
				auto* FileInfo = GetSelectedFileInfo(hooks::UpdateButtons::ActiveBrowserRef);

				if (!FileInfo || FileInfo->FileType != SDK::EUGCFileType::Online) {
					//For some reason the button refrence defualts on other buttons. The extra check should verify that an online file is selected (workshop).
					S_OnClicked->OriginalFunction(This);
					return;
				}

				if (Blacklist::Get()->IsVehicleSteamLinkPositive(*FileInfo))
				{
					if (!GetBlacklistButton()) return;
					Blacklist::Get()->RemoveBannedSteamLink(*FileInfo);
					GetBlacklistButton()->SetColorStyle(SDK::EBrickUIColorStyle::Default);
					GetBlacklistButton()->SetDisplayText(TEXT(ADD_VEHICLE_BUTTON_MESSAGE));
				} 
				else 
				{
					if (!GetBlacklistButton()) return;
					Blacklist::Get()->AddBannedSteamLink(*FileInfo);
					GetBlacklistButton()->SetColorStyle(SDK::EBrickUIColorStyle::Negative);
					GetBlacklistButton()->SetDisplayText(TEXT(REMOVE_VEHICLE_BUTTON_MESSAGE));
				}
			}
			else {
				S_OnClicked->OriginalFunction(This);
			}
		}

		OnClicked() : Hook(HOnClicked, HookedFunction) {}
	};
}