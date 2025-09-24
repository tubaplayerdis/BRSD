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
};

struct BlockedPlayer
{
	PlayerInfo Blocker;
	PlayerInfo Blocked;

	BlockedPlayer() {
		Blocker = PlayerInfo();
		Blocked = PlayerInfo();
	}

	BlockedPlayer(std::string blocker, std::string blocked) {
		Blocker = PlayerInfo(blocker);
		Blocked = PlayerInfo(blocked);
	}

	BlockedPlayer(PlayerInfo blocker, PlayerInfo blocked) {
		Blocker = PlayerInfo(blocker);
		Blocked = PlayerInfo(blocked);
	}

	bool operator==(const BlockedPlayer& other) const {
		return (Blocker == other.Blocker && Blocked == other.Blocked);
	}
};