#include "Elevator.h"
#include <SDK.hpp>
#include "utils.h"
#include "uibase.h"
#include "Hook.h"
#include "offsets.h"
#include <print>
#include "global.h"
#include "moderation.h"

Elevator* S_Elevator = nullptr;
SDK::UWBP_MenuButton_C* ElevateButtonRef = nullptr;
SDK::UWBP_MenuButton_C* MutePlayerRef = nullptr;
#define ICON_INDEX_SHIELD 13
#define ICON_INDEX_X 11
#define ICON_INDEX_PLUS 19
#define DE_ELEVATE_TEXT TEXT(L"De-Elevate Player")
#define ELEVATE_TEXT TEXT(L"Elevate Player")
#define MUTE_PLAYER_TEXT TEXT(L"Mute Player")
#define UNMUTE_PLAYER_TEXT TEXT(L"Un-Mute Player")

//TODO: Implement saving muted and admin players to a file so they can be elevated/muted upon joining.

HOOK(OnPlayerClicked, HOnPlayerClicked, [](SDK::UScoreboardWidget* This, SDK::UScoreboardPlayerWidget* PlayerWidget) -> void
{
	HOOK_CALL_ORIGINAL(OnPlayerClicked(), This, PlayerWidget);
	if (!PlayerWidget->PlayerState) return;

	if (ElevateButtonRef && PlayerWidget->PlayerState->IsHost()) {
		ElevateButtonRef->SetColorStyle(SDK::EBrickUIColorStyle::Negative);
		ElevateButtonRef->SetDisplayText(DE_ELEVATE_TEXT);
		ElevateButtonRef->SetDisplayIcon(ICON_INDEX_X);//Makes visual changes to signify that the host is an admin
		ElevateButtonRef->SetIsEnabled(false);
	}
	else if (ElevateButtonRef && PlayerWidget->PlayerState->IsAdmin())
	{
		ElevateButtonRef->SetColorStyle(SDK::EBrickUIColorStyle::Negative);
		ElevateButtonRef->SetDisplayText(DE_ELEVATE_TEXT);
		ElevateButtonRef->SetDisplayIcon(ICON_INDEX_X);
		ElevateButtonRef->SetIsEnabled(true);
	}
	else if (ElevateButtonRef && !PlayerWidget->PlayerState->IsAdmin())
	{
		ElevateButtonRef->SetColorStyle(SDK::EBrickUIColorStyle::Neutral);
		ElevateButtonRef->SetDisplayText(ELEVATE_TEXT);
		ElevateButtonRef->SetDisplayIcon(ICON_INDEX_SHIELD);
		ElevateButtonRef->SetIsEnabled(true);
	}

	PlayerInfo ActivePlayer = GetPlayerInfoFromState(PlayerWidget->PlayerState);

	if (MutePlayerRef && PlayerWidget->PlayerState->IsHost())
	{
		MutePlayerRef->SetColorStyle(SDK::EBrickUIColorStyle::Default);
		MutePlayerRef->SetDisplayText(MUTE_PLAYER_TEXT);
		MutePlayerRef->SetDisplayIcon(ICON_INDEX_PLUS);
		MutePlayerRef->SetIsEnabled(false);
	}
	else if (MutePlayerRef && moderation::isPlayerMuted(ActivePlayer))
	{
		MutePlayerRef->SetColorStyle(SDK::EBrickUIColorStyle::Negative);
		MutePlayerRef->SetDisplayText(UNMUTE_PLAYER_TEXT);
		MutePlayerRef->SetDisplayIcon(ICON_INDEX_X);
		MutePlayerRef->SetIsEnabled(true);
	}
	else if (MutePlayerRef && !moderation::isPlayerMuted(ActivePlayer))
	{
		MutePlayerRef->SetColorStyle(SDK::EBrickUIColorStyle::Default);
		MutePlayerRef->SetDisplayText(MUTE_PLAYER_TEXT);
		MutePlayerRef->SetDisplayIcon(ICON_INDEX_PLUS);
		MutePlayerRef->SetIsEnabled(true);
	}
}, void(SDK::UScoreboardWidget*, SDK::UScoreboardPlayerWidget*));


Elevator::Elevator() : Module(false)
{
	HOOK_INIT(OnPlayerClicked());
	HOOK_ENABLE(OnPlayerClicked());
}

//Called from an OpenMenu route
void Elevator::CreateButton()
{
	if (!GetMenu() && !GetMenu()->GetButtonPanel()) return;
	SDK::TDelegate<void()> del = SDK::TDelegate<void()>();
	ElevateButtonRef = static_cast<SDK::UWBP_MenuButton_C*>(GetMenu()->GetButtonPanel()->CreateButton(ELEVATE_TEXT, ICON_INDEX_SHIELD, del));//19 is the index of the plus icon
	ElevateButtonRef->SetIsEnabled(false);
	ElevateButtonRef->SetColorStyle(SDK::EBrickUIColorStyle::Neutral);

	SDK::TDelegate<void()> del2 = SDK::TDelegate<void()>();
	MutePlayerRef = static_cast<SDK::UWBP_MenuButton_C*>(GetMenu()->GetButtonPanel()->CreateButton(MUTE_PLAYER_TEXT, ICON_INDEX_PLUS, del2));
	MutePlayerRef->SetIsEnabled(false);
	MutePlayerRef->SetColorStyle(SDK::EBrickUIColorStyle::Default);
}

//Called in hooking file.
void Elevator::DestroyHooks()
{
	//HOOK_DESTROY(OnPlayerClicked());
}

void Elevator::NullifyRefrence()
{
	ElevateButtonRef = nullptr;
	MutePlayerRef = nullptr;
}

bool Elevator::DoesButtonRefMatch(void* Ref, ElevatorButtons but)
{
	switch (but)
	{
		case ElevateButton:
			if (!ElevateButtonRef) return false;
			return Ref == ElevateButtonRef;
		case MuteToggleButton:
			if (!MutePlayerRef) return false;
			return Ref == MutePlayerRef;
		default:
			break;
	}
	return false;
}

void Elevator::OnElevateButtonClicked()
{
	if(!GetMenu() || !GetMenu()->CurrentMenuPage || !GetMenu()->CurrentMenuPage->IsA(UCLASS(SDK::UWBP_PlayersMenu_C))) return;
	SDK::UWBP_PlayersMenu_C* PlayersMenu = Cast<SDK::UWBP_PlayersMenu_C*>(GetMenu()->CurrentMenuPage);
	SDK::ABrickPlayerState* State = PlayersMenu->Scoreboard->SelectedPlayerState.Get();
	if (!State) return;
	if (State->IsHost()) return;//What are you doing to cause this.
	if (State->IsAdmin())
	{
		State->SetIsAdmin(false);
		//Only do visual feedback when sucsessfull
		if (!State->IsAdmin())
		{
			ElevateButtonRef->SetColorStyle(SDK::EBrickUIColorStyle::Neutral);
			ElevateButtonRef->SetDisplayText(ELEVATE_TEXT);
			ElevateButtonRef->SetDisplayIcon(ICON_INDEX_SHIELD);
			ElevateButtonRef->SetIsEnabled(true);
		}
	}
	else
	{
		State->SetIsAdmin(true);
		//Only do visual feedback when sucsessfull
		if (State->IsAdmin())
		{
			ElevateButtonRef->SetColorStyle(SDK::EBrickUIColorStyle::Negative);
			ElevateButtonRef->SetDisplayText(DE_ELEVATE_TEXT);
			ElevateButtonRef->SetDisplayIcon(ICON_INDEX_X);
			ElevateButtonRef->SetIsEnabled(true);
		}
	}
}

void Elevator::OnMuteToggleButtonClicked()
{
	if (!GetMenu() || !GetMenu()->CurrentMenuPage || !GetMenu()->CurrentMenuPage->IsA(UCLASS(SDK::UWBP_PlayersMenu_C))) return;
	SDK::UWBP_PlayersMenu_C* PlayersMenu = Cast<SDK::UWBP_PlayersMenu_C*>(GetMenu()->CurrentMenuPage);
	SDK::ABrickPlayerState* State = PlayersMenu->Scoreboard->SelectedPlayerState.Get();
	if (!State) return;
	PlayerInfo ActivePlayer = GetPlayerInfoFromState(State);
	if (moderation::isPlayerMuted(ActivePlayer))
	{
		moderation::RemoveMutedPlayer(ActivePlayer);
		if (!moderation::isPlayerMuted(ActivePlayer))
		{
			MutePlayerRef->SetColorStyle(SDK::EBrickUIColorStyle::Default);
			MutePlayerRef->SetDisplayText(MUTE_PLAYER_TEXT);
			MutePlayerRef->SetDisplayIcon(ICON_INDEX_PLUS);
			MutePlayerRef->SetIsEnabled(true);
		}
	}
	else
	{
		moderation::AddMutedPlayer(ActivePlayer);
		if (moderation::isPlayerMuted(ActivePlayer))
		{
			MutePlayerRef->SetColorStyle(SDK::EBrickUIColorStyle::Negative);
			MutePlayerRef->SetDisplayText(UNMUTE_PLAYER_TEXT);
			MutePlayerRef->SetDisplayIcon(ICON_INDEX_X);
			MutePlayerRef->SetIsEnabled(true);
		}
	}
}

Elevator* Elevator::Get()
{
	if (!S_Elevator) S_Elevator = new Elevator();
	return S_Elevator;
}