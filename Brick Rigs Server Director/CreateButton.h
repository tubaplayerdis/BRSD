/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     CreateButton.h                                              */
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
	namespace CreateButton
	{
		inline bool enabled = false;
		inline bool initalized = false;
		inline uintptr_t FunctionPointer = 0;//Transisiton to a signature if this becomes important in the future.

		using  Function_t = SDK::UMenuButtonWidget*(__fastcall*)(SDK::UMenuButtonPanelWidget* This, SDK::FText* DisplayText, int IconIndex, void* Delegate);
		inline  Function_t OriginalFunction = nullptr;

		SDK::UMenuButtonWidget* __fastcall HookedFunction(SDK::UMenuButtonPanelWidget* This, SDK::FText* DisplayText, int IconIndex, void* Delegate);

		bool Init();
		void Enable();
		void Disable();
	}
}