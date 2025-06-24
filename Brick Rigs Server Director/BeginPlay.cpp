#include "BeginPlay.h"
#include "global.h"
#include "watermark.h"
#include "welcome.h"
#include "AddChatMessage.h"
#include "OnPlayerJoined.h"
#include "StartPlay.h"
#include <MinHook.h>

void __fastcall hooks::BeginPlay::HookedBeginPlayFunction(SDK::UWorld* This)
{
	OriginalBeginPlayFunction(This);
	if (!global::IsHost(This->NetDriver)) {
		//Disable Hooks
		AddChatMessage::Disable();
		OnPlayerJoined::Disable();
		StartPlay::Disable();
		watermark::HideWaterWark();
		return;
	}
	else {
		//Re-Enable hooks. Enable() function pretects agaisnt enabling twice.
		AddChatMessage::Enable();
		OnPlayerJoined::Enable();
		StartPlay::Enable();
	}
	global::pointers::UpdatePointers(This);
	if (global::isMapValid()) watermark::HideWaterWark();
	else { watermark::ShowWaterMark(); }
	Sleep(50); //This is janky but allows for execution of other systems to prevent freezes
	welcome::SendWelcomeMessageA();

}

bool hooks::BeginPlay::Init()
{
	if (initalized) return false;
	BeginPlayFunctionPointer = FindPattern(pattern, mask, GetModuleBaseN(), GetModuleSizeN());
	if (BeginPlayFunctionPointer == 0) return false;
	MH_STATUS ret = MH_CreateHook((LPVOID)BeginPlayFunctionPointer, &HookedBeginPlayFunction, (void**)&OriginalBeginPlayFunction);
	initalized = true;
	return ret == MH_OK;
}

void hooks::BeginPlay::Enable()
{
	if (!initalized || enabled) return;
	MH_EnableHook((LPVOID)BeginPlayFunctionPointer);
	enabled = true;
}

void hooks::BeginPlay::Disable()
{
	if (!initalized || !enabled) return;
	MH_DisableHook((LPVOID)BeginPlayFunctionPointer);
	enabled = false;
}