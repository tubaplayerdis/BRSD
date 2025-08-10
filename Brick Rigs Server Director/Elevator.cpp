#include "Elevator.h"
#include <SDK.hpp>
#include "uibase.h"
#include "Hook.h"
#include "offsets.h"
#include <print>
#include "global.h"

Elevator* S_Elevator = nullptr;
SDK::UWBP_MenuButton_C* CurrentButtonRef = nullptr;
#define ICON_INDEX_SHIELD 13
#define ICON_INDEX_X 11

HOOK(OnPlayerClicked, HOnPlayerClicked, [](SDK::UScoreboardWidget* This, SDK::UScoreboardPlayerWidget* PlayerWidget) -> void
{
	std::cout << "Update Can Kick Player was called!" << std::endl;
	HOOK_CALL_ORIGINAL(OnPlayerClicked(), This, PlayerWidget);
	if (!CurrentButtonRef || !PlayerWidget->PlayerState) return;

	if (PlayerWidget->PlayerState->IsHost()) {
		CurrentButtonRef->SetColorStyle(SDK::EBrickUIColorStyle::Negative);
		CurrentButtonRef->SetDisplayText(TEXT(L"De-Elevate User"));
		CurrentButtonRef->SetDisplayIcon(ICON_INDEX_X);//Makes visual changes to signify that the host is an admin
		CurrentButtonRef->SetIsEnabled(false);
	}
	else if (PlayerWidget->PlayerState->IsAdmin()) 
	{
		CurrentButtonRef->SetColorStyle(SDK::EBrickUIColorStyle::Negative);
		CurrentButtonRef->SetDisplayText(TEXT(L"De-Elevate User"));
		CurrentButtonRef->SetDisplayIcon(ICON_INDEX_X);
		CurrentButtonRef->SetIsEnabled(true);
	}
	else if (!PlayerWidget->PlayerState->IsAdmin())
	{
		CurrentButtonRef->SetColorStyle(SDK::EBrickUIColorStyle::Neutral);
		CurrentButtonRef->SetDisplayText(TEXT(L"Elevate User"));
		CurrentButtonRef->SetIsEnabled(true);
	}
}, void(SDK::UScoreboardWidget*, SDK::UScoreboardPlayerWidget*));


Elevator::Elevator() : Module(false)
{
	HOOK_INIT(OnPlayerClicked());
	HOOK_ENABLE(OnPlayerClicked());
	std::cout << "Enabled Hook!" << std::endl;
}

//Called from an OpenMenu route
void Elevator::CreateButton()
{
	if (!GetMenu() && !GetMenu()->GetButtonPanel()) return;
	SDK::TDelegate<void()> del = SDK::TDelegate<void()>();
	CurrentButtonRef = static_cast<SDK::UWBP_MenuButton_C*>(GetMenu()->GetButtonPanel()->CreateButton(TEXT(L"Elevate User"), ICON_INDEX_SHIELD, del));//19 is the index of the plus icon
	CurrentButtonRef->SetIsEnabled(false);
	//GetMenu()->GetButtonPanel()->CreateButton();
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
	//TODO: Implement elevating users based on the scoreboard widget. Get the menu and then the active widget.
}

Elevator* Elevator::Get()
{
	if (!S_Elevator) S_Elevator = new Elevator();
	return S_Elevator;
}