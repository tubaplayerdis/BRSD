/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     modules.h			                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once

#include "messages.h"
#include "welcome.h"
#include "interpreter.h"
#include "moderation.h"
#include "Blacklist.h"

namespace modules
{
	//Also enabled certian default startup modules
	inline void InitalizeModules()
	{
		CreateModule(Blacklist, M_Blacklist);
		EnableModule(M_Blacklist);
	}
	
	inline void DestroyModules()
	{
		DestroyModule(M_Blacklist);
	}
}