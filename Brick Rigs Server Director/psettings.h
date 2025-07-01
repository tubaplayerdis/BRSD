/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     psettings.h		                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    25 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <SDK.hpp>
#include <vector>
namespace psettings
{
	inline SDK::UMenuPageWidget* MockPage = nullptr; //adds the backround of the menu page that looks correct
	inline SDK::UBrickScrollBox* CustomSettingsPage = nullptr;
	inline std::vector<SDK::UWidget*> ElementsList = std::vector<SDK::UWidget*>();
	bool CreateCustomSettingsPage();
	void SetVisibility(SDK::ESlateVisibility vis);
	void Uninitalize();
}