/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     OnClicked.cpp		                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    25 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "OnClicked.h"
#include <MinHook.h>
#include "obutton.h"
#include "UMenuWidget.h"
#include "psettings.h"
#include "uibase.h"
#include "global.h"
#include "UMainWidgetBase.h"

void __fastcall hooks::OnClicked::HookedFunction(SDK::UMenuButtonWidget* This)
{
	if (This == obutton::CurrentButtonRef) {
		std::cout << "BRSD Button Clicked!" << std::endl;
		if (GetMenu()->CurrentMenuPage) GetMenu()->RemoveMenuPage(GetMenu()->CurrentMenuPage);
		GetMenu()->SetTitleText(TEXT(L"BRSD"));
		UMenuWidget::OpenMenu(NAME(L"InGameMenu/BSRD"));
		GetMenu()->AddMenuPage(psettings::CustomSettingsPage);
		GetMenu()->CurrentMenuPage = psettings::CustomSettingsPage;
	} else OriginalFunction(This);
}

bool hooks::OnClicked::Init() {
	if (initalized) return false;
	FunctionPointer = (uintptr_t)GetModuleHandle(NULL) + 0x0D76780;
	if (FunctionPointer == 0) return false;
	MH_STATUS ret = MH_CreateHook((LPVOID)FunctionPointer, &HookedFunction, (void**)&OriginalFunction);
	initalized = true;
	return ret == MH_OK;
}

void hooks::OnClicked::Enable()
{
	if (!initalized || enabled) return;
	MH_EnableHook((LPVOID)FunctionPointer);
	enabled = true;
}

void hooks::OnClicked::Disable()
{
	if (!initalized || !enabled) return;
	MH_DisableHook((LPVOID)FunctionPointer);
	enabled = false;
}
