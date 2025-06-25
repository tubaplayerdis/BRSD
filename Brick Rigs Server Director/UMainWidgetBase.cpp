/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     UMainWidgetBase.cpp		                                  */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    25 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "UMainWidgetBase.h"
#include <windows.h>

void UMainWidgetBase::UpdateInputMode(SDK::UMainWidgetBase* Base)
{
	uintptr_t UpdateInputModeFunction = (uintptr_t)GetModuleHandle(NULL) + 0x0DEEB70;

	using UpdateInputModeFn = void (__fastcall*)(SDK::UMainWidgetBase* Base);
	UpdateInputModeFn OnUpdateInputModeFunction = reinterpret_cast<UpdateInputModeFn>(UpdateInputModeFunction);

	OnUpdateInputModeFunction(Base);
}
