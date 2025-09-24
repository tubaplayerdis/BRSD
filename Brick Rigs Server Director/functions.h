/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     functions.h			                                      */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <BR-SDK.hpp>
#include "offsets.h"

namespace FBrickChatMessage
{
	inline void FBrickChatMessageConstructor(SDK::FBrickChatMessage* This, SDK::EChatMessageType ChatType, SDK::ABrickPlayerController* PC)
	{
		return CallGameFunction<void, SDK::FBrickChatMessage*, SDK::EChatMessageType, SDK::ABrickPlayerController*>(FFBrickChatMessageConstructor, This, ChatType, PC);
	}
}

inline void SynchronizeProperties(SDK::UBrickBorder* This)
{
	return CallGameFunction<void, SDK::UBrickBorder*>(UBRICKBORDER_SYNCHRONIZEPROPERTIES, This);
}