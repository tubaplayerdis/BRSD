/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     uibase.cpp			                                      */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "uibase.h"
#include "global.h"
#include "Function.h"

SDK::UWBP_WindowManager_C* GetWindowManager()
{
	return static_cast<SDK::UWBP_WindowManager_C*>(SDK::UWindowManagerWidget::Get(World()));
}

SDK::UPanelWidget* GetRootWidgetPanel()
{
	return static_cast<SDK::UPanelWidget*>(GetWindowManager()->WidgetTree->RootWidget);
}

SDK::UCanvasPanel* GetCanvasPanel()
{
	if (!global::World) return nullptr;
	SDK::UCanvasPanel* panel = nullptr;
	SDK::UPanelWidget* RootPanel = GetRootWidgetPanel();
	if (!RootPanel) return nullptr;
	for (int i = 0; i < RootPanel->GetChildrenCount(); ++i) {
		SDK::UWidget* child = RootPanel->GetChildAt(i);
		if (child->IsA(SDK::UCanvasPanel::StaticClass())) {
			panel = static_cast<SDK::UCanvasPanel*>(child);
			return panel;
		}
	}
	return nullptr;
}

SDK::UWBP_Menu_C* GetMenu()
{
	if (!global::World) return nullptr;
	SDK::UWBP_Menu_C* MainMenu = nullptr;
	SDK::UCanvasPanel* panel = GetCanvasPanel();
	if (!panel) return nullptr;
	for (int i = 0; i < panel->GetChildrenCount(); i++) {
		if (panel->GetChildAt(i)->IsA(SDK::UWBP_Menu_C::StaticClass())) {
			MainMenu = static_cast<SDK::UWBP_Menu_C*>(panel->GetChildAt(i));
			return MainMenu;
		}
	}
	return nullptr;
}

void uibase::PrintButtonsInfo()
{
	SDK::UWBP_Menu_C* menu = GetMenu();
	if (menu) {
		std::cout << "Buttons Used: " << menu->GetButtonPanel()->NumButtonsUsed << std::endl;
		std::cout << "Buttons Per Row: " << menu->GetButtonPanel()->NumButtonsPerRow << std::endl;
		std::cout << "Num Buttons Array: " << menu->GetButtonPanel()->Buttons.Num() << std::endl;
	}
}

std::string uibase::GetCurrentMenu()
{
	if (!GetMenu()) return std::string("None");
	if (GetMenu()->CurrentMenuPage != nullptr)
	{
		std::cout << "CMenuPageNotNull" << std::endl;
		return GetMenu()->CurrentMenuPage->GetName();
	}
	else return std::string("None");
}

bool uibase::IsButtonMenuVisible()
{
	SDK::UWBP_Menu_C* menu = GetMenu();
	if (!menu) return false;
	SDK::UMenuButtonPanelWidget* panel = menu->GetButtonPanel();
	if (!panel) return false;
	return panel->GetIsVisible();
}

bool uibase::IsInGameMenuOpen()
{
	return IsButtonMenuVisible && global::isMapValid();
}

void uibase::Cleanup()
{
	SDK::UWBP_Menu_C* menu = GetMenu();
	if (menu) {
		SDK::UMenuButtonPanelWidget* panel = menu->GetButtonPanel();
		if (panel && panel->Buttons.Num() == 8) {
			panel->NumButtonsUsed = 7;
			panel->RemoveUnusedButtons();
		}
	}
}

SDK::UUserWidget* WidgetU::CreateWidget(SDK::UWorld* World, SDK::TSubclassOf<SDK::UUserWidget> UserWidgetClass, SDK::FName WidgetName)
{
	if (addy == 0)
	{
		std::cout << "Calculating first time widget creation" << std::endl;
		unsigned long long base = (unsigned long long)GetModuleHandle(NULL);
		MODULEINFO modInfo = {};
		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &modInfo, sizeof(modInfo));
		unsigned __int64 size = modInfo.SizeOfImage;
		unsigned __int64 patternLen = strlen(mask);
		for (unsigned __int64 i = 0; i < size - patternLen; i++) {
			bool found = true;

			for (unsigned __int64 j = 0; j < patternLen; j++) {
				if (mask[j] != '?' && signature[j] != *(char*)(base + i + j)) {
					found = false;
					break;
				}
			}

			if (found) {
				addy = base + i;
				break;
			}

		}
	}

	if (addy == 0) { std::cout << "boom!" << std::endl; return nullptr; }

	return CallGameFunctionO<SDK::UUserWidget*, SDK::UWorld*, SDK::TSubclassOf<SDK::UUserWidget>, SDK::FName>(addy, World, UserWidgetClass, WidgetName);
}
