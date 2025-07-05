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
#include "functions.h"

namespace hooks
{
	class OnClicked;
	inline OnClicked* S_OnClicked = nullptr; //Non-Inline causes link 2005

	class OnClicked : public Hook<void, SDK::UMenuButtonWidget*>
	{
	public:

		static void __fastcall HookedFunction(SDK::UMenuButtonWidget* This)
		{
			if (GetMenu()->CurrentMenuPage && GetMenu()->CurrentMenuPage == psettings::MockPage) {
				psettings::SetHook(false);
				static_cast<SDK::UBrickBorder*>(psettings::CustomSettingsPage->Slot->Parent)->SetContent(psettings::MockPage);
			}
			if (This == obutton::CurrentButtonRef) {
				if (GetMenu()->CurrentMenuPage) GetMenu()->RemoveMenuPage(GetMenu()->CurrentMenuPage);
				const SDK::FText title = TEXT(L"BRSD");
				UMenuWidget::OpenMenu(NAME(L"InGameMenu/BSRD"));
				GetMenu()->AddMenuPage(psettings::MockPage);
				GetMenu()->CurrentMenuPage = psettings::MockPage;
				std::cout << psettings::MockPage->Slot->GetName() << std::endl;
				SDK::UBorderSlot* slot = static_cast<SDK::UBorderSlot*>(psettings::MockPage->Slot);
				if (!slot) 
				{
					std::cout << "slot no" << std::endl;
					return;
				}
				if (!slot->Parent)
				{
					std::cout << "parent no" << std::endl;
					return;
				}
				SDK::UBrickBorder* Parent = static_cast<SDK::UBrickBorder*>(slot->Parent);
				Parent->SetContent(psettings::CustomSettingsPage);
				psettings::PrepareCustomSettingsPage();
				psettings::SetVisibility(SDK::ESlateVisibility::Visible);
				psettings::SetHook(true);
			}
			else {
				S_OnClicked->OriginalFunction(This);
			}
		}

		OnClicked() : Hook(HOnClicked, HookedFunction) {}
	};
}