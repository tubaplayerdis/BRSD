/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
<<<<<<<< HEAD:Brick Rigs Server Director/uibase.h
/*    Module:     uibase.h			                                          */
========
/*    Module:     Saver.h				                                      */
>>>>>>>> 7cacd3db24a7f8ac6be2a50bce642027b0aa9424:Brick Rigs Server Director/Saver.h
/*    Author:     Aaron Wilk                                                  */
/*    Created:    12 July 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
<<<<<<<< HEAD:Brick Rigs Server Director/uibase.h
#include <SDK.hpp>

namespace uibase
{
	void PrintButtonsInfo();
	std::string GetCurrentMenu();
	bool IsButtonMenuVisible();
	bool IsInGameMenuOpen();

	void Cleanup();
}

SDK::UWBP_WindowManager_C* GetWindowManager();
SDK::UPanelWidget* GetRootWidgetPanel();
SDK::UCanvasPanel* GetCanvasPanel();
SDK::UWBP_Menu_C* GetMenu();
========
#include "Module.h"
#include <vector>

class Saver : public Module
{
	Saver() : Module(false) {}

	inline bool Enable() override
	{
		//enablization function
	}

	inline bool Disable() override
	{
		//disable stuff
	}

};
>>>>>>>> 7cacd3db24a7f8ac6be2a50bce642027b0aa9424:Brick Rigs Server Director/Saver.h
