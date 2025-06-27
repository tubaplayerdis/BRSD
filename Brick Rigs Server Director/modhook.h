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
#include "Hook.h"

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

	namespace checks
	{
		class ForEachMod;
		inline ForEachMod* S_ForEachMod = nullptr; //Non-Inline causes link 2005

		class ForEachMod : public Hook<void, void*, void*>
		{
		public:

			static void __fastcall HookedFunction(void* WorldContext, void* Function)
			{
				std::cout << "For Each Mod Hook was Called!" << std::endl;
				S_ForEachMod->OriginalFunction(WorldContext, Function); //The original function is filled in when the class is created.
			}

			ForEachMod() : Hook(0x0D1F6C0, HookedFunction) {}
		};

		class OverrideMenu;
		inline OverrideMenu* S_OverrideMenuHook = nullptr; //Non-Inline causes link 2005

		class OverrideMenu : public Hook<bool, SDK::UModHook*, SDK::UMenuWidget*, const SDK::FName*>
		{
		public:

			static bool __fastcall HookedFunction(SDK::UModHook* This, SDK::UMenuWidget* Widget, const SDK::FName* Context)
			{
				std::cout << "Override Menu was called!" << std::endl;
				return S_OverrideMenuHook->OriginalFunction(This, Widget, Context); //The original function is filled in when the class is created.
			}

			OverrideMenu() : Hook(0x0EA1990, HookedFunction) {}
		};
	}
}
