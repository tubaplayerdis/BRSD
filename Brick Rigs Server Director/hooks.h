/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     hooks.h			                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once

//Include all hook files
#include "AddChatMessage.h"
#include "BeginPlay.h"
#include "DrawTransition.h"
#include "LoadMap.h"
#include "OnPlayerJoined.h"
#include "OpenMenu.h"
#include "StartPlay.h"
#include "CreateButton.h"

namespace hooks
{
	bool InitHooks();

	void EnableHooks();

	void OpenCrashFile();
}
