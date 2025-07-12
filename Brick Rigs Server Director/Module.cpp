/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     Module.cpp												  */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    12 July 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "Module.h"

Module::Module(bool NeedsInitalization, bool(*vInitilizationFunction)(void))
{
	bIsEnabled = false;
	bIsInitalized = false;
	bNeedsInitalization = NeedsInitalization;
	if (bNeedsInitalization && vInitilizationFunction)
	{
		bIsInitalized = vInitilizationFunction();
	}
}
