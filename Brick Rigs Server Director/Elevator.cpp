#include "Elevator.h"
#include <SDK.hpp>
#include "uibase.h"
#include "Hook.h"
#include "offsets.h"
#include <print>
#include <SDK.hpp>

Elevator* S_Elevator = nullptr;



HOOK(OnPlayerKicked, HOnPlayerClicked, [](SDK::UScoreboardWidget* This, SDK::UScoreboardPlayerWidget* PlayerWidget) -> void
{
	std::cout << "Update Can Kick Player was called!" << std::endl;
	H_OnPlayerKicked->CallOriginalFunction(This, PlayerWidget);	
}, void(SDK::UScoreboardWidget*, SDK::UScoreboardPlayerWidget*))


Elevator::Elevator() : Module(false)
{
	HOOK_INIT(H_OnPlayerKicked);
	if (!H_OnPlayerKicked->IsInitialized()) std::cout << "Hook Failed!" << std::endl;
	HOOK_ENABLE(H_OnPlayerKicked);
	std::cout << "Enabled Hook!" << std::endl;
}

void Elevator::CreateButton()
{
	if (!GetMenu() && !GetMenu()->GetButtonPanel()) return;
	//GetMenu()->GetButtonPanel()->CreateButton();
}

Elevator* Elevator::Get()
{
	if (!S_Elevator) S_Elevator = new Elevator();
	return S_Elevator;
}