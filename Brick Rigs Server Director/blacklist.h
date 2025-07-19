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
#include <SDK/BrickRigs_structs.hpp>

//For blacklist, it should be by name, steam link or file.

class Blacklist : public Module
{
	std::vector<std::string> vBannedVehicleKeywords;

public:
	Blacklist() : Module(false, nullptr) 
	{
		vBannedVehicleKeywords.push_back("lol");
	}

	inline bool Enable() override
	{
		SetEnable(true);
		return true;
	}

	inline bool Disable() override
	{
		SetEnable(false);
		return true;
	}

	inline bool IsVehicleBannned(SDK::FUGCFileInfo vehicle)
	{
		for (std::string veh : vBannedVehicleKeywords)
		{
			if (std::string::npos != vehicle.Title.ToString().find(veh)) return true;
		}
		return false;
	}
};

inline Blacklist* M_Blacklist = nullptr;