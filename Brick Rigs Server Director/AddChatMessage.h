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
#include "SDK.hpp"
#include "windows.h"

namespace hooks
{
	namespace AddChatMessage
	{
		inline bool enabled = false;
		inline bool initalized = false;
		inline const char* pattern = "\x48\x89\x7C\x24\x18\x41\x56\x48\x83\xEC\x40\x48\x8B\xFA\x4C\x8B\xF1\xE8\x00\x00\x00\x00\x84\xC0\x0F\x84\x00\x00\x00\x00";
		inline const char* mask = "xxxxxxxxxxxxxxxxxx????xxxx????";
		inline uintptr_t FunctionPointer = 0;

		using  Function_t = void(__fastcall*)(SDK::ABrickGameSession* This, const SDK::FBrickChatMessage& ChatMessage);
		inline  Function_t OriginalFunction = nullptr;

		void __fastcall HookedFunction(SDK::ABrickGameSession* This, const SDK::FBrickChatMessage& ChatMessage);

		bool Init();
		void Enable();
		void Disable();
	}
}
