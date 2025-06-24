#include "LoadMap.h"
#include "global.h"
#include "watermark.h"
#include <MinHook.h>

char __fastcall hooks::LoadMap::HookedLoadMapFunction(SDK::UEngine* This, SDK::FWorldContext* WorldContext, void* URL, void* Pending, void* Error)
{
	char ret = OriginalLoadMapFunction(This, WorldContext, URL, Pending, Error);
	global::mapLevelName = World()->PersistentLevel->Outer->GetName();
	if (global::mapLevelName == "MainMenu") watermark::ShowWaterMark();
	Sleep(20);
	return ret;
}

bool hooks::LoadMap::Init()
{
	if (initalized) return false;
	LoadMapFunctionPointer = FindPattern(pattern, mask, GetModuleBaseN(), GetModuleSizeN());
	if (LoadMapFunctionPointer == 0) return false;
	MH_STATUS ret = MH_CreateHook((LPVOID)LoadMapFunctionPointer, &HookedLoadMapFunction, (void**)&OriginalLoadMapFunction);
	initalized = true;
	return ret == MH_OK;
}

void hooks::LoadMap::Enable()
{
	if (!initalized || enabled) return;
	MH_EnableHook((LPVOID)LoadMapFunctionPointer);
	enabled = true;
}

void hooks::LoadMap::Disable()
{
	if (!initalized || !enabled) return;
	MH_DisableHook((LPVOID)LoadMapFunctionPointer);
	enabled = false;
}
