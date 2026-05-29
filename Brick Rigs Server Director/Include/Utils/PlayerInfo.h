/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     PlayerInfo.h		                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <SDK/BrickRigs_classes.hpp>
#include "../Global/Global.h"
#include "string"

struct PlayerInfo
{
	std::string name;

	PlayerInfo() {
		name = "None";
	}

	PlayerInfo(std::string cname) {
		name = cname;
	}

	bool operator==(const PlayerInfo& other) const {
		return other.name == name;
	}

	SDK::ABrickPlayerController* GetController() const
	{
		return GetBrickPlayerControllerFromName(name);
	}

	SDK::ABrickCharacter* GetCharacter() const
	{
		return reinterpret_cast<SDK::ABrickCharacter*>(GetBrickPlayerControllerFromName(name)->GetPlayerCharacter());
	}

	SDK::ABrickPlayerState* GetPlayerState() const
	{
		return reinterpret_cast<SDK::ABrickPlayerState*>(GetBrickPlayerControllerFromName(name)->PlayerState);
	}

	static PlayerInfo GetPlayerInfoFromController(SDK::ABrickPlayerController* controller)
	{
		if (controller == nullptr) return PlayerInfo();
		PlayerInfo ret = PlayerInfo();
		SDK::ABrickPlayerState* state = static_cast<SDK::ABrickPlayerState*>(controller->PlayerState);
		ret.name = state->GetPlayerNameText().ToString();
		return ret;
	}

	static PlayerInfo GetPlayerInfoFromState(SDK::ABrickPlayerState* State)
	{
		return GetPlayerInfoFromController(GetBrickPlayerControllerFromID(std::to_string(State->PlayerId)));
	}
};