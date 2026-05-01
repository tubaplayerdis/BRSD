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
#include "../../Utils/Offsets.h"
#include <windows.h>
#include <BR-SDK.hpp>
#include "../../Global/Global.h"

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
            return S_RestartPlayerAt->OriginalFunction(This, Player, Request);
        }

        RestartPlayerAt() : Hook(HRestartPlayerAt, HookedFunction) {}
    };
}