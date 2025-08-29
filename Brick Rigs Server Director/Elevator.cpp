#include "Elevator.h"
#include <BR-SDK.hpp>
#include "uibase.h"
#include "offsets.h"
#include <print>
#include "global.h"
#include "moderation.h"

#define ELEVATE_TEXT TEXT(L"Elevate Player")
#define MUTE_PLAYER_TEXT TEXT(L"Mute Player")
#define UNMUTE_PLAYER_TEXT TEXT(L"Un-Mute Player")

std::unique_ptr<Elevator> S_Elevator = nullptr;

//TODO: Implement saving muted and admin players to a file so they can be elevated/muted upon joining.

HOOK(OnPlayerClicked, HOnPlayerClicked, [](SDK::UScoreboardWidget* This, SDK::UScoreboardPlayerWidget* PlayerWidget) -> void
{
	HOOK_CALL_ORIGINAL(OnPlayerClicked(), This, PlayerWidget);
	if (!PlayerWidget->PlayerState) return;

	if (PlayerWidget->PlayerState->IsHost()) {
		
	}
	else if (PlayerWidget->PlayerState->IsAdmin())
	{
		
	}
	else if (!PlayerWidget->PlayerState->IsAdmin())
	{
		
	}

	PlayerInfo ActivePlayer = GetPlayerInfoFromState(PlayerWidget->PlayerState);

	if (PlayerWidget->PlayerState->IsHost())
	{
		
	}
	else if (moderation::isPlayerMuted(ActivePlayer))
	{
		
	}
	else if (!moderation::isPlayerMuted(ActivePlayer))
	{
		
	}
}, void(SDK::UScoreboardWidget*, SDK::UScoreboardPlayerWidget*));


Elevator::Elevator() : Module(false)
{
	HOOK_INIT(OnPlayerClicked());
	HOOK_ENABLE(OnPlayerClicked());
}

void Elevator::CreateMenu()
{
}

void Elevator::RemoveMenu()
{
}

void ElevateUser()
{
	if(!GetMenu() || !GetMenu()->CurrentMenuPage || !GetMenu()->CurrentMenuPage->IsA(GetUClass(SDK::UWBP_PlayersMenu_C))) return;
	SDK::UWBP_PlayersMenu_C* PlayersMenu = Cast<SDK::UWBP_PlayersMenu_C>(GetMenu()->CurrentMenuPage);
	SDK::ABrickPlayerState * State = GetMember<SDK::TWeakObjectPtr<SDK::ABrickPlayerState>>(PlayersMenu->Scoreboard, 0x270).Get();
	if (!State) return;
	if (State->IsHost()) return;//What are you doing to cause this.
	if (State->IsAdmin())
	{
		State->SetIsAdmin(false);
		//Only do visual feedback when sucsessfull
		if (!State->IsAdmin())
		{
			//Just De-Elevated
		}
	}
	else
	{
		State->SetIsAdmin(true);
		//Only do visual feedback when sucsessfull
		if (State->IsAdmin())
		{
			//Just Elevated
		}
	}
}

void MutePlayer()
{
	if (!GetMenu() || !GetMenu()->CurrentMenuPage || !GetMenu()->CurrentMenuPage->IsA(GetUClass(SDK::UWBP_PlayersMenu_C))) return;
	SDK::UWBP_PlayersMenu_C* PlayersMenu = Cast<SDK::UWBP_PlayersMenu_C>(GetMenu()->CurrentMenuPage);
	SDK::ABrickPlayerState* State = GetMember<SDK::TWeakObjectPtr<SDK::ABrickPlayerState>>(PlayersMenu->Scoreboard, 0x270).Get();
	if (!State) return;
	PlayerInfo ActivePlayer = GetPlayerInfoFromState(State);
	if (moderation::isPlayerMuted(ActivePlayer))
	{
		moderation::RemoveMutedPlayer(ActivePlayer);
		if (!moderation::isPlayerMuted(ActivePlayer))
		{
			//Player Un-Muted
		}
	}
	else
	{
		moderation::AddMutedPlayer(ActivePlayer);
		if (moderation::isPlayerMuted(ActivePlayer))
		{
			//Player Muted
		}
	}
}

Elevator* Elevator::Get()
{
	if (!S_Elevator) S_Elevator = std::make_unique<Elevator>();
	return S_Elevator.get();
}