/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     Blacklist.h			                                      */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    12 July 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include "Module.h"
#include <vector>

//For blacklist, it should be by name, steam link or file.

class Blacklist : public Module
{
public:
	Blacklist() : Module(false, nullptr) {}

	std::vector<std::string> vBannedVehicleNames;

	inline bool Enable() override
	{
		SetEnable(true);
	}

	inline bool Disable() override
	{
		SetEnable(false);
	}

	inline bool IsVehicleBannned(std::string vehiclename)
	{
		for (std::string veh : vBannedVehicleNames)
		{
			if (veh == vehiclename) return true;
		}
		return false;
	}
};

inline Blacklist* M_Blacklist = nullptr;