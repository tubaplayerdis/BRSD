/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     Saver.h				                                      */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    12 July 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include "Module.h"
#include <vector>

class Saver : public Module
{
	Saver() : Module(false) {}

	inline bool Enable() override
	{
		//enablization function
	}

	inline bool Disable() override
	{
		//disable stuff
	}

};