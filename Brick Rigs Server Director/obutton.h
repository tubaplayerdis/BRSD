/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     obutton.h			                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    25 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <SDK.hpp>

namespace obutton
{
	inline SDK::UWBP_MenuButton_C* CurrentButtonRef = nullptr;
	void AddToButtonMenu();
}