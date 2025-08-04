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
#include "Function.h"
#include "offsets.h"
#include <vector>
#include <string>
#include <SDK.hpp>

struct FFloat16
{
	union
	{
		unsigned __int16 Encoded;
		// Possibly also:
		struct
		{
			uint16_t Mantissa : 10;
			uint16_t Exponent : 5;
			uint16_t Sign : 1;
		} Components;
	};
};

//For blacklist, it should be by name, steam link or file.

class Blacklist : public Module
{
	std::vector<std::string> vBannedVehicleKeywords;
	std::vector<std::wstring> vBannedSteamLinks;

public:
	Blacklist() : Module(false, nullptr) 
	{
		vBannedVehicleKeywords.push_back("lol");
		vBannedSteamLinks.push_back(L"https://steamcommunity.com/sharedfiles/filedetails/?id=2994703505");
	}

	inline void AddBannedKeyword(std::string keyword)
	{
		vBannedVehicleKeywords.push_back(keyword);
	}

	inline void AddBannedSteamLink(SDK::FUGCFileInfo vehicle)
	{
		SDK::TArray<FFloat16> ret = SDK::TArray<FFloat16>();
		CallGameFunction<SDK::TArray<FFloat16>*, void*, SDK::TArray<FFloat16>*>(FFFluUGCItemIdWrapperToString, &vehicle.OnlineItemId, &ret);
		std::wstringstream builder;
		for (FFloat16 num : ret)
		{
			builder << (wchar_t)num.Encoded;
		}
		std::wstring steamLink = builder.str();
		if (steamLink == L"INVALID") return;//Not a steam item, is local.
		//Now the string is like: Steam:8993473328
		if (steamLink.find_first_of(':') == std::wstring::npos) return;//IDK what could cause this
		steamLink = steamLink.substr(steamLink.find_first_of(':') + 1);
		steamLink.insert(0, L"https://steamcommunity.com/sharedfiles/filedetails/?id=");
		//The steam link should now be formatted to how it is seen in the browser.
		vBannedSteamLinks.push_back(steamLink);
	}

	inline void RemoveBannedSteamLink(SDK::FUGCFileInfo vehicle)
	{
		SDK::TArray<FFloat16> ret = SDK::TArray<FFloat16>();
		CallGameFunction<SDK::TArray<FFloat16>*, void*, SDK::TArray<FFloat16>*>(FFFluUGCItemIdWrapperToString, &vehicle.OnlineItemId, &ret);
		std::wstringstream builder;
		for (FFloat16 num : ret)
		{
			builder << (wchar_t)num.Encoded;
		}
		std::wstring steamLink = builder.str();
		if (steamLink == L"INVALID") return;//Not a steam item, is local.
		//Now the string is like: Steam:8993473328
		if (steamLink.find_first_of(':') == std::wstring::npos) return;//IDK what could cause this
		steamLink = steamLink.substr(steamLink.find_first_of(':') + 1);
		steamLink.insert(0, L"https://steamcommunity.com/sharedfiles/filedetails/?id=");
		//The steam link should now be formatted to how it is seen in the browser.

		for (int i = 0; i < vBannedSteamLinks.size(); i++)
		{
			if (std::string::npos != steamLink.find(vBannedSteamLinks[i])) vBannedSteamLinks.erase(vBannedSteamLinks.begin() + i);
		}
	}

	inline bool IsVehicleNamePositive(SDK::FUGCFileInfo vehicle)
	{
		for (std::string veh : vBannedVehicleKeywords)
		{
			if (std::string::npos != vehicle.Title.ToString().find(veh)) return true;
		}
		return false;
	}

	inline bool IsVehicleSteamLinkPositive(SDK::FUGCFileInfo vehicle)
	{
		SDK::TArray<FFloat16> ret = SDK::TArray<FFloat16>();
		CallGameFunction<SDK::TArray<FFloat16>*, void*, SDK::TArray<FFloat16>*>(FFFluUGCItemIdWrapperToString, &vehicle.OnlineItemId, &ret);
		std::wstringstream builder;
		for (FFloat16 num : ret)
		{
			builder << (wchar_t)num.Encoded;
		}
		std::wstring steamLink = builder.str();
		if (steamLink == L"INVALID") return false;//Not a steam item, is local.
		//Now the string is like: Steam:8993473328
		if (steamLink.find_first_of(':') == std::wstring::npos) return false;//IDK what could cause this
		steamLink = steamLink.substr(steamLink.find_first_of(':')+1);
		steamLink.insert(0, L"https://steamcommunity.com/sharedfiles/filedetails/?id=");
		//The steam link should now be formatted to how it is seen in the browser.
		for (std::wstring link : vBannedSteamLinks)
		{
			if (std::string::npos != steamLink.find(link)) return true;
		}
		return false;
	}

	inline bool IsVehicleBannned(SDK::FUGCFileInfo vehicle)
	{
		if (IsDisabled()) return false;
		
		if (IsVehicleNamePositive(vehicle)) return true;

		if (IsVehicleSteamLinkPositive(vehicle)) return true;

		return false;
	}

public:
	static Blacklist* Get();
};

inline Blacklist* M_Blacklist = nullptr;

inline Blacklist* Blacklist::Get()
{
	return M_Blacklist;
}