/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     welcome.h			                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include "../../Global/global.h"
#include "../Command Engine/messages.h"
#include "../../Utils/stringlist.h"

namespace welcome
{
	inline void SendWelcomeMessage() {
		if (global::isMapValid()) {
			messages::sendUserSpecificMessageWithContext(GetBrickPlayerController(), WelcomeServerMessage, SDK::EChatContext::Global, BRSD);
			messages::sendUserSpecificMessageWithContext(GetBrickPlayerController(), ControlsHelpMessage, SDK::EChatContext::Global, BRSD);
		}
	}
	inline void SendWelcomeMessageA() {
		messages::sendUserSpecificMessageWithContext(GetBrickPlayerController(), WelcomeServerMessage, SDK::EChatContext::Global, BRSD);
		messages::sendUserSpecificMessageWithContext(GetBrickPlayerController(), ControlsHelpMessage, SDK::EChatContext::Global, BRSD);

	}
}