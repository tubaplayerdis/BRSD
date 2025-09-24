#pragma once
#include <BR-SDK.hpp>
#include "../Global/global.h"

struct FModuleManager
{
	uint8_t padding[0x3D8];
};

static_assert(sizeof(FModuleManager) == 0x3D8);

namespace offsets
{
	uintptr_t get_dll_handle = BASE + 0x1024180;
	uintptr_t module_manager_get = BASE + 0x0FF8D80;
	uintptr_t load_module_func = BASE + 0x0FFB870;
}

inline void load_module()
{
	void* dll_handle = CallGameFunction<void*, const wchar_t*>(offsets::get_dll_handle, L"C:/Users/aaron/source/repos/tubaplayerdis/BRSD/Include/ModuleUtils/UE4Editor-BR_SDK_Utilities.dll");
	SDK::TOptional<FModuleManager>* manager = CallGameFunction<SDK::TOptional<FModuleManager>*>(offsets::module_manager_get);
	FModuleManager* manager_new = reinterpret_cast<FModuleManager*>(&manager->StoredValue.Value);
	void* modulegoon = CallGameFunction<void*, FModuleManager*, SDK::FName>(offsets::load_module_func, manager_new, NAME(L"UE4Editor-BR_SDK_Utilities.dll"));
}

/*
//
//.text:0000000140FF8D80 ; TOptional<FModuleManager> *__fastcall FModuleManager::Get()
//.text:0000000140FFB870 ; IModuleInterface *__fastcall FModuleManager::LoadModule(FModuleManager *this, const FName InModuleName)
//C:\Users\aaron\source\repos\tubaplayerdis\BRSD\Include\ModuleUtils\UE4Editor-BR_SDK_Utilities.dll
*/