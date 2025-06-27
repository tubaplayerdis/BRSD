/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     UMenuWidget.h		                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    25 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <SDK.hpp>
#include "Function.h"
#include "uibase.h"

namespace UMenuWidget
{
	inline void OpenMenu(SDK::FName InMenu) 
	{
		return CallGameFunction<void, SDK::UMenuWidget*, SDK::FName>(0x0D7AD00, GetMenu(), InMenu);
	}
}