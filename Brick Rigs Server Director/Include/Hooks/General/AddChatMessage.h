/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     AddChatMessage.h                                            */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include "../../Utils/Offsets.h"
#include <BR-SDK.hpp>
#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include "../../Global/Global.h"

namespace hooks
{
	class AddChatMessage;
	inline AddChatMessage* S_AddChatMessage = nullptr; //Non-Inline causes link 2005

	class AddChatMessage : public Hook<void(SDK::ABrickGameSession*, const SDK::FBrickChatMessage&)>
	{
	public:

		static void __fastcall HookedFunction(SDK::ABrickGameSession* This, const SDK::FBrickChatMessage& ChatMessage)
		{
            S_AddChatMessage->OriginalFunction(This, ChatMessage);
		}

		AddChatMessage() : Hook(ABRICKGAMESESSION_ADDCHATMESSAGE, HookedFunction) {}
	};
}
