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
	inline SDK::UMenuPageWidget* CustomSettingsPage = nullptr;
	inline std::vector<SDK::UWidget*> ElementsList = std::vector<SDK::UWidget*>();
	bool CreateCustomSettingsPage();
	void SetVisibilityOfElements(SDK::ESlateVisibility vis);
	void Uninitalize();
}