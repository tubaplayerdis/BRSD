/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     BeginPlay.h		                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/


#pragma once
#include <SDK.hpp>
#include "modules.h"
#include "global.h"
#include "AddChatMessage.h"
#include "OnPlayerJoined.h"
#include "StartPlay.h"
#include "watermark.h"
#include "Hook.h"

namespace hooks
{
	class BeginPlay;
	inline BeginPlay* S_BeginPlay = nullptr; //Non-Inline causes link 2005

	class BeginPlay : public Hook<void, SDK::UWorld*>
	{
	public:

		static void __fastcall HookedFunction(SDK::UWorld* This)
		{
			S_BeginPlay->OriginalFunction(This);
			if (!global::IsHost(This->NetDriver)) {
				//Disable Hooks
				S_AddChatMessage->Disable();
				S_OnPlayerJoined->Disable();
				S_StartPlay->Disable();
				watermark::HideWaterWark();
				return;
			}
			else {
				//Re-Enable hooks. Enable() function pretects agaisnt enabling twice.
				S_AddChatMessage->Enable();
				S_OnPlayerJoined->Enable();
				S_StartPlay->Enable();
			}
			global::pointers::UpdatePointers(This);
			if (global::isMapValid()) watermark::HideWaterWark();
			else { watermark::ShowWaterMark(); }
			Sleep(50); //This is janky but allows for execution of other systems to prevent freezes
			welcome::SendWelcomeMessageA();
			std::cout << "Started server on map: " << SDK::UGameplayStatics::GetCurrentLevelName(This, true).ToString() << std::endl;
		}

		BeginPlay() : Hook(BeginPlay_S, BeginPlay_M, HookedFunction) {}
	};
}