#pragma once

//Include all hook files
#include "AddChatMessage.h"
#include "BeginPlay.h"
#include "DrawTransition.h"
#include "LoadMap.h"
#include "OnPlayerJoined.h"
#include "OpenMenu.h"
#include "StartPlay.h"

namespace hooks
{
	bool InitHooks();

	void EnableHooks();

	void OpenCrashFile();
}
