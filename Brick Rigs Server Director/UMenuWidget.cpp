#include "UMenuWidget.h"
#include <windows.h>
#include "uibase.h"

void UMenuWidget::OpenMenu(SDK::FName InMenu)
{
	uintptr_t GetModHookSubsystemFunction = (uintptr_t)GetModuleHandle(NULL) + 0x0D7AD00;

	using GetModHookSubsystemFn = void(__fastcall*)(SDK::UMenuWidget* This, SDK::FName InMenu);
	GetModHookSubsystemFn OnGetModHookSubsystemFunction = reinterpret_cast<GetModHookSubsystemFn>(GetModHookSubsystemFunction);

	OnGetModHookSubsystemFunction(GetMenu(), InMenu);
}
