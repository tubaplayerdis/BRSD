/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     interpreter.h                                               */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once

#include "PlayerInfo.h"
#include <string>
#include <vector>

namespace interpreter
{
	namespace Commands
	{
		//Management
		void Toggle(PlayerInfo info, std::string command, bool toggle);
		void PersonalMessage(PlayerInfo info, std::string originalMessage);
		void Help(PlayerInfo info, std::string arg);

		//Movement
		bool Fly(PlayerInfo info);
		bool Walk(PlayerInfo info);
		bool Teleport(PlayerInfo info, std::string other);
		bool Ghost(PlayerInfo info);

		//Enviroment
		bool Night(PlayerInfo info);
		bool Day(PlayerInfo info);
		bool Rain(PlayerInfo info);
		bool Sun(PlayerInfo info);

		//Weapons
		bool AmmoType(PlayerInfo info, std::string ammotype);

		//Misc
		void Debug(PlayerInfo info); //Use this to test varius functions. Can be called by imgui.

		void Uninject(PlayerInfo info);

		namespace Moderation
		{
			void ToggleMute(PlayerInfo info, std::string originalMessage, bool on_of);
			void ToggleBlock(PlayerInfo info, std::string originalMessage, bool on_off);
			void ToggleSilence(PlayerInfo info, bool on_off);
			void Save(PlayerInfo info);
			void Load(PlayerInfo info);
			void ListMuted(PlayerInfo info);
			void ListBlocked(PlayerInfo info);
			void IsOnSilence(PlayerInfo info);
			void ListPlayerIDS(PlayerInfo info);
		}

		//Toggles for wether or not a command is enabled or disabled

		inline bool isPM = true;

		inline bool isFly = true;
		inline bool isWalk = true;
		inline bool isTeleport = true;
		inline bool isGhost = true;

		inline bool isNight = true;
		inline bool isDay = true;
		inline bool isRain = true;
		inline bool isSun = true;

		inline bool isAmmoType = true;
	}
	void interpretCommand(std::string command, std::vector<std::string> args, PlayerInfo info, std::string originalMessage);

	std::string getAmmoTypeString(std::string ammotype);
}