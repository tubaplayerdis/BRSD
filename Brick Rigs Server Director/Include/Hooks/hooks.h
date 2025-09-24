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
#include "General/AddChatMessage.h"
#include "General/BeginPlay.h"
#include "General/LoadMap.h"
#include "General/OnPlayerJoined.h"
#include "UI/OpenMenu.h"
#include "General/StartPlay.h"
#include "Vehicle/RestartPlayerAt.h"

namespace hooks
{
	bool InitHooks();

	void EnableHooks();

	void DestroyHookObjects();

	void OpenCrashFile();
}
