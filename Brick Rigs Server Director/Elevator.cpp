#include "Elevator.h"
#include <SDK.hpp>
#include "utils.h"
#include "uibase.h"
#include "Hook.h"
#include "offsets.h"
#include <print>
#include "global.h"

Elevator* S_Elevator = nullptr;
SDK::UWBP_MenuButton_C* CurrentButtonRef = nullptr;
#define ICON_INDEX_SHIELD 13
#define ICON_INDEX_X 11
#define DE_ELEVATE_TEXT TEXT(L"De-Elevate User")
#define ELEVATE_TEXT TEXT(L"Elevate User")

HOOK(OnPlayerClicked, HOnPlayerClicked, [](SDK::UScoreboardWidget* This, SDK::UScoreboardPlayerWidget* PlayerWidget) -> void
{
	HOOK_CALL_ORIGINAL(OnPlayerClicked(), This, PlayerWidget);
	if (!CurrentButtonRef || !PlayerWidget->PlayerState) return;

	if (PlayerWidget->PlayerState->IsHost()) {
		CurrentButtonRef->SetColorStyle(SDK::EBrickUIColorStyle::Negative);
		CurrentButtonRef->SetDisplayText(DE_ELEVATE_TEXT);
		CurrentButtonRef->SetDisplayIcon(ICON_INDEX_X);//Makes visual changes to signify that the host is an admin
		CurrentButtonRef->SetIsEnabled(false);
	}
	else if (PlayerWidget->PlayerState->IsAdmin()) 
	{
		CurrentButtonRef->SetColorStyle(SDK::EBrickUIColorStyle::Negative);
		CurrentButtonRef->SetDisplayText(DE_ELEVATE_TEXT);
		CurrentButtonRef->SetDisplayIcon(ICON_INDEX_X);
		CurrentButtonRef->SetIsEnabled(true);
	}
	else if (!PlayerWidget->PlayerState->IsAdmin())
	{
		CurrentButtonRef->SetColorStyle(SDK::EBrickUIColorStyle::Neutral);
		CurrentButtonRef->SetDisplayText(ELEVATE_TEXT);
		CurrentButtonRef->SetDisplayIcon(ICON_INDEX_SHIELD);
		CurrentButtonRef->SetIsEnabled(true);
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
	CurrentButtonRef = static_cast<SDK::UWBP_MenuButton_C*>(GetMenu()->GetButtonPanel()->CreateButton(ELEVATE_TEXT, ICON_INDEX_SHIELD, del));//19 is the index of the plus icon
	CurrentButtonRef->SetIsEnabled(false);
	CurrentButtonRef->SetColorStyle(SDK::EBrickUIColorStyle::Neutral);
}

//Called in hooking file.
void Elevator::DestroyHooks()
{
	HOOK_DESTROY(OnPlayerClicked());
}

void Elevator::NullifyRefrence()
{
	CurrentButtonRef = nullptr;
}

bool Elevator::DoesButtonRefMatch(void* Ref)
{
	if (!CurrentButtonRef) return false;
	return Ref == CurrentButtonRef;
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
			CurrentButtonRef->SetColorStyle(SDK::EBrickUIColorStyle::Neutral);
			CurrentButtonRef->SetDisplayText(ELEVATE_TEXT);
			CurrentButtonRef->SetDisplayIcon(ICON_INDEX_SHIELD);
			CurrentButtonRef->SetIsEnabled(true);
		}
	}
	else
	{
		State->SetIsAdmin(true);
		//Only do visual feedback when sucsessfull
		if (State->IsAdmin())
		{
			CurrentButtonRef->SetColorStyle(SDK::EBrickUIColorStyle::Negative);
			CurrentButtonRef->SetDisplayText(DE_ELEVATE_TEXT);
			CurrentButtonRef->SetDisplayIcon(ICON_INDEX_X);
			CurrentButtonRef->SetIsEnabled(true);
		}
	}
}

Elevator* Elevator::Get()
{
	if (!S_Elevator) S_Elevator = new Elevator();
	return S_Elevator;
}