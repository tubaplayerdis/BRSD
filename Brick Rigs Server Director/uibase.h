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
#include "functions.h"


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
	template<typename T>
	inline SDK::UUserWidget* CreateWidget(SDK::UWorld* World, SDK::TSubclassOf<SDK::UUserWidget> UserWidgetClass, const char* WidgetClassName)
	{

		if (UserWidgetClass == nullptr) {
			std::string wcn = std::string(WidgetClassName);
			AttemptLoadClass(wcn.substr(wcn.find_first_of('U')+1).c_str());
			UserWidgetClass = T::StaticClass();
		}

		return CallGameFunction<SDK::UUserWidget*, SDK::UWorld*, SDK::TSubclassOf<SDK::UUserWidget>, SDK::FName>(FCreateWidget, World, UserWidgetClass, SDK::FName());
	}
}

SDK::UWBP_WindowManager_C* GetWindowManager();
SDK::UPanelWidget* GetRootWidgetPanel();
SDK::UCanvasPanel* GetCanvasPanel();
SDK::UWBP_Menu_C* GetMenu();
