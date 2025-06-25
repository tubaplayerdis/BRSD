#include "obutton.h"
#include "global.h"
#include <SDK.hpp>
#include "uibase.h"

void obutton::AddToButtonMenu()
{
	SDK::UWBP_Menu_C* MainMenu = GetMenu();
	if (!MainMenu) return;
	if (MainMenu->GetButtonPanel()->Buttons.Num() > 0) {
		SDK::TDelegate<void()> del = MainMenu->GetButtonPanel()->Buttons[0]->OnClickedDelegate;
		MainMenu->GetButtonPanel()->CreateButton(TEXT(L"BRSD Options"), 12, del);
	}
}
