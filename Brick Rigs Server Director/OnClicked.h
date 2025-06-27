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
#include "SDK.hpp"
#include "windows.h"
#include "Hook.h"
#include "global.h"
#include "obutton.h"
#include "uibase.h"
#include "SetTitleText.h"
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
			if (This == obutton::CurrentButtonRef) {
				std::cout << "BRSD Button Clicked!" << std::endl;
				if (GetMenu()->CurrentMenuPage) GetMenu()->RemoveMenuPage(GetMenu()->CurrentMenuPage);
				const SDK::FText title = TEXT(L"BRSD");
				if (hooks::S_SetTitleText) SetTitleText::HookedFunction(GetMenu(), &title);
				UMenuWidget::OpenMenu(NAME(L"InGameMenu/BSRD"));
				GetMenu()->AddMenuPage(psettings::CustomSettingsPage);
				GetMenu()->CurrentMenuPage = psettings::CustomSettingsPage;
			}
			else S_OnClicked->OriginalFunction(This);
		}

		OnClicked() : Hook(0x0D76780, HookedFunction) {}
	};
}