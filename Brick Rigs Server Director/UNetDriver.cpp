/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     UNetDriver.cpp	                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "UNetDriver.h"

bool UNetDriver::isServer(SDK::UNetDriver* driver)
{
	if (!driver) return false;
	using IsServerFn = bool(__fastcall*)(SDK::UNetDriver*);
	void** vtable = *(void***)driver;
	IsServerFn IsServerFunc = reinterpret_cast<IsServerFn>(vtable[0x378 / 8]);
	return IsServerFunc(driver);
}
