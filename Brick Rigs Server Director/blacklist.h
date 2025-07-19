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

//For blacklist, it should be by name, steam link or file.

class Blacklist : public Module
{
	Blacklist() : Module(false, nullptr) {}

	inline bool Enable() override
	{
		//enable what needs to be in blacklist namespace
	}

	inline bool Disable() override
	{
		//disable stuff from blacklist namespace
	}
};

inline Blacklist* M_Blacklist = nullptr;