#include "OnPlayerJoined.h"
#include <MinHook.h>
#include "global.h"
#include "messages.h"
#include "stringlist.h"

void __fastcall hooks::OnPlayerJoined::HookedOnPlayerJoinedFunction(SDK::ABrickGameSession* This, SDK::ABrickPlayerController* PC)
{
	OriginalOnPlayerJoinedFunction(This, PC);
	messages::sendUserSpecificMessageWithContext(GetPlayerInfoFromController(PC), WelcomeClientMessage, SDK::EChatContext::Global, L"Welcome!");
}

bool hooks::OnPlayerJoined::Init()
{
	if (initalized) return false;
	OnPlayerJoinedFunctionPointer = FindPattern(pattern, mask, GetModuleBaseN(), GetModuleSizeN());
	if (OnPlayerJoinedFunctionPointer == 0) return false;
	MH_STATUS ret = MH_CreateHook((LPVOID)OnPlayerJoinedFunctionPointer, &HookedOnPlayerJoinedFunction, (void**)&OriginalOnPlayerJoinedFunction);
	initalized = true;
	return ret == MH_OK;
}

void hooks::OnPlayerJoined::Enable()
{
	if (!initalized || enabled) return;
	MH_EnableHook((LPVOID)OnPlayerJoinedFunctionPointer);
	enabled = true;
}

void hooks::OnPlayerJoined::Disable()
{
	if (!initalized || !enabled) return;
	MH_DisableHook((LPVOID)OnPlayerJoinedFunctionPointer);
	enabled = false;
}
