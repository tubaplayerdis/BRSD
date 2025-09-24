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

#include "Command Engine/messages.h"
#include "Server Director/welcome.h"
#include "Command Engine/interpreter.h"
#include "Server Director/moderation.h"
#include "Server Director/Blacklist.h"

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