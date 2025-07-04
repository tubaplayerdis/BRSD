/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     uibase.h			                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <SDK.hpp>

namespace uibase
{
	void PrintButtonsInfo();
	std::string GetCurrentMenu();
	bool IsButtonMenuVisible();
	bool IsInGameMenuOpen();

	void Cleanup();
}

namespace WidgetU
{
	SDK::UUserWidget* CreateWidget(SDK::UWorld* World, SDK::TSubclassOf<SDK::UUserWidget> UserWidgetClass, SDK::FName WidgetName);
}

SDK::UWBP_WindowManager_C* GetWindowManager();
SDK::UPanelWidget* GetRootWidgetPanel();
SDK::UCanvasPanel* GetCanvasPanel();
SDK::UWBP_Menu_C* GetMenu();
