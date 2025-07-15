/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     blacklist.h			                                      */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    12 July 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include "Module.h"

class BLModule : public Module
{
	BLModule() : Module(false, nullptr) {}

	inline bool Enable() override
	{
		//enable what needs to be in blacklist namespace
	}

	inline bool Disable() override
	{
		//disable stuff from blacklist namespace
	}
};

namespace blacklist
{
	//vehicle blacklist
	//hook the spawning function
}