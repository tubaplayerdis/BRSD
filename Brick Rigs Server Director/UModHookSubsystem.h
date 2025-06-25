/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     UModHookSubsystem.h                                         */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    25 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <SDK.hpp>

namespace UModHookSubsystem
{
	SDK::UModHookSubsystem* GetModHookSubsystem();
	void CreateModHooks(SDK::UModHookSubsystem* This);
}