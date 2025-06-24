/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     moderation.h		                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <vector>
#include "PlayerInfo.h"

namespace moderation
{
	inline std::vector<PlayerInfo> MutedPlayers = std::vector<PlayerInfo>();
	inline std::vector<PlayerInfo> PlayersOnPMSilence = std::vector<PlayerInfo>();
	inline std::vector<BlockedPlayer> BlockedPlayers = std::vector<BlockedPlayer>();

	bool isPlayerBlockedBy(PlayerInfo blocker, PlayerInfo blocked);
	bool isPlayerMuted(PlayerInfo player);
	bool isPlayerOnSilence(PlayerInfo player);

	//basic set and getters
	bool AddMutedPlayer(PlayerInfo info);
	bool RemoveMutedPlayer(PlayerInfo info);
	bool AddPMSilencePlayer(PlayerInfo info);
	bool RemovePMSilencePlayer(PlayerInfo info);
	bool AddBlockedPlayer(BlockedPlayer info);
	bool RemoveBlockedPlayer(BlockedPlayer info);

	bool saveModerationValues();
	bool loadModerationValues();
}