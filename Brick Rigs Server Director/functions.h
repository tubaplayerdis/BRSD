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
#include <SDK.hpp>
#include "Function.h"
#include "uibase.h"
#include "offsets.h"

namespace FLinearColor
{
	/*
	* Colors are 0-255 and A is 0-1
	* FLinearColor expect values from 0-1 not 0-255, given that, 0-255 values are taken and mapped down in the function.
	*/
	inline SDK::FLinearColor CreateLinearColor(float R, float G, float B, float A)
	{
		SDK::FLinearColor ret = SDK::FLinearColor();
		ret.A = A;
		ret.R = R / 255.0f;
		ret.G = G / 255.0f;
		ret.B = B / 255.0f;
		return ret;
	}


	/*
	* Colors are 0-1 and A is 0-1
	* FLinearColor expect values from 0-1 not 0-255, and since this is the normal function, inputs are 0-1.
	*/
	inline SDK::FLinearColor CreateLinearColorNorm(float R, float G, float B, float A)
	{
		SDK::FLinearColor ret = SDK::FLinearColor();
		ret.A = A;
		ret.R = R;
		ret.G = G;
		ret.B = B;
		return ret;
	}
}

namespace FSlateColor
{
	/*Use this by passing in a valid linear color and a FSlateColor created by the default constructor*/
	inline void FSlateColorConstructor(SDK::FSlateColor* This, SDK::FLinearColor* InColor)
	{
		return CallGameFunction<void, SDK::FSlateColor*, SDK::FLinearColor*>(FindPatternF("\x0F\x10\x02\x33\xC0\x0F\x11\x01\x48\x89\x41\x18\x48\x89\x41\x20\x48\x8B\xC1\xC6\x41\x10\x00\xC3", "xxxxxxxxxxxxxxxxxxxxxxxx"), This, InColor);
	}

	/*
	* Colors are 0-255 and A is 0-1
	*/
	inline SDK::FSlateColor CreateSlateColor(float R, float G, float B, float A)
	{
		SDK::FSlateColor ret;
		SDK::FLinearColor col = FLinearColor::CreateLinearColor(R, G, B, A);
		FSlateColorConstructor(&ret, &col);
		return ret;
	}
}

namespace FBrickChatMessage
{
	inline void FBrickChatMessageConstructor(SDK::FBrickChatMessage* This, SDK::EChatMessageType ChatType, SDK::ABrickPlayerController* PC)
	{
		return CallGameFunction<void, SDK::FBrickChatMessage*, SDK::EChatMessageType, SDK::ABrickPlayerController*>(0x0D0EA10, This, ChatType, PC);
	}
}

inline void SynchronizeProperties(SDK::UBrickBorder* This)
{
	return CallGameFunction<void, SDK::UBrickBorder*>(OSynchronizeProperties_UBB, This);
}

inline void SynchronizeProperties(SDK::UBrickScrollBox* This)
{
	return CallGameFunction<void, SDK::UBrickScrollBox*>(OSynchronizeProperties_UBSB, This);
}

inline char Initalize(SDK::UUserWidget* widget)
{
	if (!widget) return false;
	using InitalizeFn = bool(__fastcall*)(SDK::UUserWidget*);
	void** vtable = *(void***)widget;
	InitalizeFn InitalizeFunc = reinterpret_cast<InitalizeFn>(vtable[VOInitalize]);
	return InitalizeFunc(widget);
}



namespace UMainWidgetBase
{
	inline void UpdateInputMode(SDK::UMainWidgetBase* Base)
	{
		return CallGameFunction<void, SDK::UMainWidgetBase*>(OUpdateInputMode, Base);
	}
}

namespace UMenuWidget
{
	inline void OpenMenu(SDK::FName InMenu)
	{
		return CallGameFunction<void, SDK::UMenuWidget*, SDK::FName>(OOpenMenu, GetMenu(), InMenu);
	}
}

namespace UModHookSubsystem
{
	inline SDK::UModHookSubsystem* GetModHookSubsystem() 
	{
		return CallGameFunction<SDK::UModHookSubsystem*>(OGetModHookSubsystem);
	}
}

namespace UNetDriver
{
	inline bool isServer(SDK::UNetDriver* driver)
	{
		if (!driver) return false;
		using IsServerFn = bool(__fastcall*)(SDK::UNetDriver*);
		void** vtable = *(void***)driver;
		IsServerFn IsServerFunc = reinterpret_cast<IsServerFn>(VOisServer);
		return IsServerFunc(driver);
	}
}