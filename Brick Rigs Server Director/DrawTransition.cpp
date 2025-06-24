#include "DrawTransition.h"
#include "FLinearColor.h"
#include "global.h"
#include <MinHook.h>

void __fastcall hooks::DrawTransition::HookedDrawTransitionFunction(SDK::UGameViewportClient* This, SDK::UCanvas* Canvas)
{
	SDK::FVector2D loc = SDK::FVector2D();
	loc.X = 100;
	loc.Y = 100;
	SDK::FVector2D size = SDK::FVector2D();
	size.X = 100;
	size.Y = 100;
	Canvas->K2_DrawBox(loc, size, 10, FLinearColor::CreateLinearColor(255, 0, 0, 1));
	OriginalDrawTransitionFunction(This, Canvas);
}

bool hooks::DrawTransition::Init()
{
	if (initalized) return false;
	DrawTransitionFunctionPointer = FindPattern(pattern, mask, GetModuleBaseN(), GetModuleSizeN());
	if (DrawTransitionFunctionPointer == 0) return false;
	MH_STATUS ret = MH_CreateHook((LPVOID)DrawTransitionFunctionPointer, &HookedDrawTransitionFunction, (void**)&OriginalDrawTransitionFunction);
	initalized = true;
	return ret == MH_OK;
}

void hooks::DrawTransition::Enable()
{
	if (!initalized || enabled) return;
	MH_EnableHook((LPVOID)DrawTransitionFunctionPointer);
	enabled = true;
}

void hooks::DrawTransition::Disable()
{
	if (!initalized || !enabled) return;
	MH_DisableHook((LPVOID)DrawTransitionFunctionPointer);
	enabled = false;
}
