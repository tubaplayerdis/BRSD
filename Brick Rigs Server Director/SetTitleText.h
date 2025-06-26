/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     SetTitleText.h	                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    26 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <SDK.hpp>
#include "Hook.h"

namespace hooks
{
	class SetTitleText;
	inline SetTitleText* SetTitleTextSingleton = nullptr; //Non-Inline causes link 2005

	class SetTitleText : public Hook<void, SDK::UMenuWidget*, const SDK::FText*>
	{
	public:

		static void __fastcall HookedFunction(SDK::UMenuWidget* This, const SDK::FText* Name)
		{
			std::cout << "Setting Title Text: " << Name->ToString() << std::endl;
			SetTitleTextSingleton->OriginalFunction(This, Name); //The original function is filled in when the class is created.
		}

		SetTitleText() : Hook(0x0EA2170, HookedFunction) {}
	};
}