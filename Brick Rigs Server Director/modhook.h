/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     modhook.h			                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    25 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <SDK.hpp>

namespace modhook
{
	bool CreateBRSDModHook();
	void Uninitialize();

	bool OverrideMenu_Impl(SDK::UModHook* This, SDK::UMenuWidget* Widget, const SDK::FName& Context);

	namespace hooked
	{
		inline bool enabled = false;
		inline bool initalized = false;
		inline uintptr_t FunctionPointer = 0;//Transisiton to a signature if this becomes important in the future.

		using  Function_t = void(__fastcall*)(SDK::UModHook* This);
		inline  Function_t OriginalFunction = nullptr;

		void __fastcall HookedFunction(SDK::UModHook* This);

		bool Init();
		void Enable();
		void Disable();
	}
}
