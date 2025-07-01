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
				if (GetMenu()->CurrentMenuPage) GetMenu()->RemoveMenuPage(GetMenu()->CurrentMenuPage);
				const SDK::FText title = TEXT(L"BRSD");
				UMenuWidget::OpenMenu(NAME(L"InGameMenu/BSRD"));
				GetMenu()->AddMenuPage(psettings::MockPage);
				GetMenu()->CurrentMenuPage = psettings::MockPage;
				psettings::SetVisibility(SDK::ESlateVisibility::Visible);
			}
			else {
				psettings::SetVisibility(SDK::ESlateVisibility::Hidden);
				S_OnClicked->OriginalFunction(This);
			}
		}

		OnClicked() : Hook(0x0D76780, HookedFunction) {}
	};
}