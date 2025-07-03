/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     AddChatMessage.h                                            */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <SDK.hpp>
#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include "messages.h"
#include "interpreter.h"
#include "moderation.h"
#include "global.h"
#include "Hook.h"

namespace hooks
{
	class AddChatMessage;
	inline AddChatMessage* S_AddChatMessage = nullptr; //Non-Inline causes link 2005

	class AddChatMessage : public Hook<void, SDK::ABrickGameSession*, const SDK::FBrickChatMessage&>
	{
	public:

		static void __fastcall HookedFunction(SDK::ABrickGameSession* This, const SDK::FBrickChatMessage& ChatMessage)
		{
            PlayerInfo info;
            info.name = ChatMessage.Player.PlayerName.ToString();

            if (moderation::isPlayerMuted(info)) { ContextMessage(info, "You are currently muted.", SDK::EChatContext::Admin, L"Admin"); return; }

            if (ChatMessage.Type == SDK::EChatMessageType::Message && ChatMessage.TextOption.ToString().at(0) == '/') {
                std::string raw = ChatMessage.TextOption.ToString();
                std::string command = "";//grab the word after the slash and ending at the space.
                std::vector<std::string> args = std::vector<std::string>(); //Grab all strings passed after the first one as seperated by a space
                if (raw.length() > 2) {
                    int numSpaces = std::count(raw.begin(), raw.end(), ' ');
                    if (numSpaces == 0) {
                        //no arguments
                        command = raw.substr(0);
                    }
                    else {
                        //arguments
                        command = raw.substr(0, raw.find_first_of(' '));
                        std::string rawArgs = raw.substr(raw.find_first_of(' '));
                        std::istringstream iss(rawArgs);
                        std::string word;

                        while (iss >> word) {
                            word.erase(std::remove(word.begin(), word.end(), ' '));
                            args.push_back(word);
                        }
                    }
                }
                if (global::IsHost()) {
                    interpreter::interpretCommand(command, args, info, raw);
                    return; //To not send out peoples commands to everyone else on the server
                }
            }
            S_AddChatMessage->OriginalFunction(This, ChatMessage);
		}

        //Dosent work with the optimized search algo
		AddChatMessage() : Hook("\x48\x89\x7C\x24\x18\x41\x56\x48\x83\xEC\x40\x48\x8B\xFA\x4C\x8B\xF1\xE8\x00\x00\x00\x00\x84\xC0\x0F\x84\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxx????xxxx????", HookedFunction, SAFE) {}
	};
}
