/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     OnClicked.h		                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    25 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include "SDK.hpp"
#include "windows.h"
namespace hooks
{
	namespace OnClicked
	{
		inline bool enabled = false;
		inline bool initalized = false;
		inline uintptr_t FunctionPointer = 0;//Transisiton to a signature if this becomes important in the future.

		using  Function_t = void (__fastcall*)(SDK::UMenuButtonWidget* This);
		inline  Function_t OriginalFunction = nullptr;

		void __fastcall HookedFunction(SDK::UMenuButtonWidget* This);

		bool Init();
		void Enable();
		void Disable();
	}
}