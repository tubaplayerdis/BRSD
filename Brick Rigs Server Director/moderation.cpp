/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     moderation.cpp	                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "moderation.h"
#include <fstream>
#include <iostream>

bool moderation::isPlayerBlockedBy(PlayerInfo blocker, PlayerInfo blocked)
{
	std::cout << "Searching in blocked users!" << std::endl;
	BlockedPlayer player = BlockedPlayer(blocked, blocker); //This is reversed for some reason. This should work.
	for (BlockedPlayer blocked : BlockedPlayers) {
		if (player == blocked) return true;
	}
	return false;
}

bool moderation::isPlayerMuted(PlayerInfo player)
{
	for (PlayerInfo info : MutedPlayers) {
		if (player == info) return true;
	}
	return false;
}

bool moderation::isPlayerOnSilence(PlayerInfo player)
{
	for (PlayerInfo info : PlayersOnPMSilence) {
		if (player == info) return true;
	}
	return false;
}

bool moderation::AddMutedPlayer(PlayerInfo info)
{
	if (isPlayerMuted(info)) return false;
	MutedPlayers.push_back(info);
	return true;
}

bool moderation::RemoveMutedPlayer(PlayerInfo info)
{
	size_t old = MutedPlayers.size();
	MutedPlayers.erase(std::remove(MutedPlayers.begin(), MutedPlayers.end(), info), MutedPlayers.end());
	size_t news = MutedPlayers.size();
	return (news < old);
}

bool moderation::AddPMSilencePlayer(PlayerInfo info)
{
	if (isPlayerOnSilence(info)) return false;
	PlayersOnPMSilence.push_back(info);
	return true;
}

bool moderation::RemovePMSilencePlayer(PlayerInfo info)
{
	size_t old = PlayersOnPMSilence.size();
	PlayersOnPMSilence.erase(std::remove(PlayersOnPMSilence.begin(), PlayersOnPMSilence.end(), info), PlayersOnPMSilence.end());
	size_t news = PlayersOnPMSilence.size();
	return (news < old);
}

bool moderation::AddBlockedPlayer(BlockedPlayer info)
{
	if (isPlayerBlockedBy(info.Blocker, info.Blocked)) return false;
	BlockedPlayers.push_back(info);
	return true;
}

bool moderation::RemoveBlockedPlayer(BlockedPlayer info)
{
	size_t old = BlockedPlayers.size();
	BlockedPlayers.erase(std::remove(BlockedPlayers.begin(), BlockedPlayers.end(), info), BlockedPlayers.end());
	size_t news = BlockedPlayers.size();
	return (news < old);
}

bool moderation::saveModerationValues()
{
	std::fstream saveFile;
	saveFile.open("MutedPlayers.txt", std::ios::out);
	if (!saveFile.bad() && !saveFile.fail() && saveFile.is_open()) {
		for (PlayerInfo info : MutedPlayers) {
			saveFile << info.name << std::endl;
		}
		saveFile.close();
		return true;
	}
	return false;
}

bool moderation::loadModerationValues()
{
	std::fstream saveFile;
	saveFile.open("MutedPlayers.txt", std::ios::in);
	if (!saveFile.bad() && !saveFile.fail() && saveFile.is_open()) {
		std::string line;
		while (std::getline(saveFile, line)) {
			AddMutedPlayer(PlayerInfo(line));
		}
		return true;
	}
	return false;
}
