#pragma once
#include "global.h"
#include "messages.h"
#include "stringlist.h"

namespace welcome
{
	inline void SendWelcomeMessage() {
		if (global::isMapValid()) {
			messages::sendUserSpecificMessageWithContext(GetPlayerInfoFromController(GetBrickPlayerController()), WelcomeServerMessage, SDK::EChatContext::Global, BRSD);
			messages::sendUserSpecificMessageWithContext(GetPlayerInfoFromController(GetBrickPlayerController()), ControlsHelpMessage, SDK::EChatContext::Global, BRSD);
		}
	}
	inline void SendWelcomeMessageA() {
		messages::sendUserSpecificMessageWithContext(GetPlayerInfoFromController(GetBrickPlayerController()), WelcomeServerMessage, SDK::EChatContext::Global, BRSD);
		messages::sendUserSpecificMessageWithContext(GetPlayerInfoFromController(GetBrickPlayerController()), ControlsHelpMessage, SDK::EChatContext::Global, BRSD);

	}
}