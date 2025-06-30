/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     OnPlayerJoined.h                                            */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <windows.h>
#include <SDK.hpp>
#include "messages.h"
#include "stringlist.h"
#include "global.h"
#include "Hook.h"
#include <fstream>

inline void LogPlayerJoining(PlayerInfo info)
{
    std::cout << "Player: " << info.name << " joined the server." << std::endl;
    UC::TArray<SDK::AActor*>* what = AllActorsOfClass(SDK::ABrickPlayerController::StaticClass());
    std::ofstream playerFile("players.log", std::ios::trunc);
    bool isValidFile = playerFile.is_open() && !playerFile.bad() && !playerFile.fail();
    playerFile << "Players on server at :" << GetCurrentTimeFormatted() << "\n";
    for (int i = 0; i < what->Num(); i++)
    {
        SDK::ABrickPlayerController* cast = static_cast<SDK::ABrickPlayerController*>(what->operator[](i));
        if (cast->PlayerState)
        {
            SDK::ABrickPlayerState* state = static_cast<SDK::ABrickPlayerState*>(cast->PlayerState);
            if (state && isValidFile) playerFile << state->GetPlayerNameText().ToString() << "\n";
        };
    }
    playerFile.close();
}

namespace hooks
{
    class OnPlayerJoined;
    inline OnPlayerJoined* S_OnPlayerJoined = nullptr; //Non-Inline causes link 2005

    class OnPlayerJoined : public Hook<void, SDK::ABrickGameSession*, SDK::ABrickPlayerController*>
    {
    public:

        static void __fastcall HookedFunction(SDK::ABrickGameSession* This, SDK::ABrickPlayerController* PC)
        {
            S_OnPlayerJoined->OriginalFunction(This, PC);
            messages::sendUserSpecificMessageWithContext(GetPlayerInfoFromController(PC), WelcomeClientMessage, SDK::EChatContext::Global, L"Welcome!");
            Release(LogPlayerJoining(GetPlayerInfoFromController(PC));)

        }

        OnPlayerJoined() : Hook("\x48\x83\xEC\x48\x48\x89\x5C\x24\x58\x33\xDB\x48\x89\x74\x24\x68\x48\x89\x7C\x24\x40", "xxxxxxxxxxxxxxxxxxxx", HookedFunction) {}
    };
}