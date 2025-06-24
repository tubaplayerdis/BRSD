#pragma once
#include <windows.h>
#include <SDK.hpp>

namespace hooks
{
    namespace StartPlay
    {
        inline bool enabled = false;
        inline bool initalized = false;
        inline const char* pattern = "\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x8B\x89\x80\x02\x00\x00\x48\x8B\x01";
        inline const char* mask = "xxxxxxxxxxxxxxxxxxx";
        inline uintptr_t StartPlayFunctionPointer = 0;

        using StartPlay_t = void(__fastcall*)(SDK::AGameMode* This);
        inline StartPlay_t OriginalStartPlayFunction = nullptr;

        void __fastcall HookedStartPlayFunction(SDK::AGameMode* This);

        bool Init();
        void Enable();
        void Disable();
    }
}