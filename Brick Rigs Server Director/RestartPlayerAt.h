/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     RestartPlayerAt.h                                           */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include "offsets.h"
#include <windows.h>
#include <SDK.hpp>
#include "messages.h"
#include "global.h"
#include "Hook.h"
#include "Module.h"

enum EPlayerSpawnResult : uint8_t
{
    Success,
    Failure,
    UnsavedChanges,
    SentToServer,
    BrickLimitExceeded,
    MassExceeded,
    SizeExceeded,
    VehicleLimitExceeded,
    VehicleNotPublicOnline,
    VehicleNotWhitelisted,
    OnlyAdminsCanSpawn,
    InsufficientFunds,
    NoObjects,
    Cooldown,
    CorruptFile,
    ValidationFailure,
    IncompatibleVersion
};

namespace hooks
{
    class RestartPlayerAt;
    inline RestartPlayerAt* S_RestartPlayerAt = nullptr; //Non-Inline causes link 2005

    //Hook should only be enabled when the CustomSettingsMenu is present.
    class RestartPlayerAt : public Hook<EPlayerSpawnResult(SDK::ABrickGameMode*, SDK::ABrickPlayerController*, SDK::FPlayerSpawnRequest)>
    {
    public:

        static EPlayerSpawnResult __fastcall HookedFunction(SDK::ABrickGameMode* This, SDK::ABrickPlayerController* Player, SDK::FPlayerSpawnRequest Request)
        {
            Sleep(50);
            if (Request.VehicleSpawnType == SDK::EPlayerVehicleSpawnType::ReplaceCurrent || Request.VehicleSpawnType == SDK::EPlayerVehicleSpawnType::SpawnNew)
            {
                if (M_Blacklist && M_Blacklist->IsVehicleBannned(Request.VehicleFileInfo))
                {
                    std::cout << "Player " << GetPlayerInfoFromController(Player).name << " spawned banned vehicle: " << Request.VehicleFileInfo.Title << std::endl;
                    Message(GetPlayerInfoFromController(Player), "Your vehicle could not be spawned as it is on the blacklist");
                    return VehicleNotWhitelisted;
                }
            }
            return S_RestartPlayerAt->OriginalFunction(This, Player, Request);
        }

        RestartPlayerAt() : Hook(HRestartPlayerAt, HookedFunction) {}
    };
}