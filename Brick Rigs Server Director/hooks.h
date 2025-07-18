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
#include "LoadMap.h"
#include "OnPlayerJoined.h"
#include "OpenMenu.h"
#include "StartPlay.h"
#include "OnClicked.h"
#include "SetSelectedItem.h"
#include "EngineLoopTick.h"
#include "RestartPlayerAt.h"

namespace hooks
{
	bool InitHooks();

	void EnableHooks();

	void DestroyHookObjects();

	void OpenCrashFile();
}
