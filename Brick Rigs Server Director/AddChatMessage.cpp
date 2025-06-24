#include "AddChatMessage.h"
#include "global.h"
#include "messages.h"
#include "interpreter.h"
#include "moderation.h"
#include <sstream>
#include <MinHook.h>

void __fastcall hooks::AddChatMessage::HookedFunction(SDK::ABrickGameSession* This, const SDK::FBrickChatMessage& ChatMessage)
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
    OriginalFunction(This, ChatMessage);
}

bool hooks::AddChatMessage::Init() {
	if (initalized) return false;
	FunctionPointer = FindPattern(pattern, mask, GetModuleBaseN(), GetModuleSizeN());
	if (FunctionPointer == 0) return false;
	MH_STATUS ret = MH_CreateHook((LPVOID)FunctionPointer, &HookedFunction, (void**)&OriginalFunction);
	initalized = true;
	return ret == MH_OK;
}

void hooks::AddChatMessage::Enable()
{
	if (!initalized || enabled) return;
	MH_EnableHook((LPVOID)FunctionPointer);
	enabled = true;
}

void hooks::AddChatMessage::Disable()
{
	if (!initalized || !enabled) return;
	MH_DisableHook((LPVOID)FunctionPointer);
	enabled = false;
}
