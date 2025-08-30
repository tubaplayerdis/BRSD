/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
<<<<<<<< HEAD:Brick Rigs Server Director/obutton.h
/*    Module:     obutton.h			                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    25 June 2025                                                */
========
/*    Module:     Utils.hpp 				                                  */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    7 July 2025                                                 */
>>>>>>>> 7cacd3db24a7f8ac6be2a50bce642027b0aa9424:Include/Utils.hpp
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once

<<<<<<<< HEAD:Brick Rigs Server Director/obutton.h
namespace obutton
{
	inline SDK::UWBP_MenuButton_C* CurrentButtonRef = nullptr;
	void AddToButtonMenu();
}
========
//Includes all utils.

#include "Utils/Utilities.hpp"
#include "Utils/ActorUtilities.hpp"
#include "Utils/GameFunctions.hpp"
>>>>>>>> 7cacd3db24a7f8ac6be2a50bce642027b0aa9424:Include/Utils.hpp
