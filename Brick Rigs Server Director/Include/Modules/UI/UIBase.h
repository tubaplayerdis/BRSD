/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     UIUtils.h			                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    12 July 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <BR-SDK.hpp>

namespace uibase
{
	void PrintButtonsInfo();
	std::string GetCurrentMenu();
	bool IsButtonMenuVisible();
	bool IsInGameMenuOpen();
	SDK::UWBP_Menu_C* GetBaseMenu();//Same as GetMenu()
	void Cleanup();
}

SDK::UWBP_WindowManager_C* GetWindowManager();
SDK::UPanelWidget* GetRootWidgetPanel();
SDK::UCanvasPanel* GetCanvasPanel();
SDK::UWBP_Menu_C* GetMenu();
