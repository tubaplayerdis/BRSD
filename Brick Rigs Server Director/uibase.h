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
	inline const char* signature = "\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x30\x48\x8B\xA9\x80\x01\x00\x00";
	inline const char* mask = "xxxxxxxxxxxxxxxxxxxxxxxxxxx";
	inline unsigned long long addy = 0;
	SDK::UUserWidget* CreateWidget(SDK::UWorld* World, SDK::TSubclassOf<SDK::UUserWidget> UserWidgetClass, SDK::FName WidgetName);
}

SDK::UWBP_WindowManager_C* GetWindowManager();
SDK::UPanelWidget* GetRootWidgetPanel();
SDK::UCanvasPanel* GetCanvasPanel();
SDK::UWBP_Menu_C* GetMenu();
