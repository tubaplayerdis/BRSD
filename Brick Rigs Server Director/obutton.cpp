/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     obutton.cpp		                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    25 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "obutton.h"
#include "psettings.h"
#include "global.h"
#include <SDK.hpp>
#include "uibase.h"

void obutton::AddToButtonMenu()
{
	SDK::UWBP_Menu_C* MainMenu = GetMenu();
	if (!MainMenu) return;
	if (MainMenu->GetButtonPanel()->Buttons.Num() > 0) {
		SDK::TDelegate<void()> del = MainMenu->GetButtonPanel()->Buttons[0]->OnClickedDelegate;
		del.BoundFunction.FunctionName = NAME(L"None");
		CurrentButtonRef = MainMenu->GetButtonPanel()->CreateButton(TEXT(L"BRSD Options"), 12, del);
	}
}
