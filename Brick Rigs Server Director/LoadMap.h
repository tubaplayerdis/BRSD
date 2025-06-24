#pragma once
#include <Windows.h>
#include <SDK.hpp>

namespace hooks
{
    namespace LoadMap
    {
        inline bool enabled = false;
        inline bool initalized = false;
        inline const char* pattern = "\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x80\x48\x81\xEC\x80\x01\x00\x00\x0F\x29\xBC\x24\x60\x01\x00\x00\x48\x8B\x05\x65\x77\xA7\x01";
        inline const char* mask = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
        inline uintptr_t LoadMapFunctionPointer = 0;

        using LoadMap_t = char(__fastcall*)(SDK::UEngine* This, SDK::FWorldContext* WorldContext, void* URL, void* Pending, void* Error);
        inline LoadMap_t OriginalLoadMapFunction = nullptr;

        char __fastcall HookedLoadMapFunction(SDK::UEngine* This, SDK::FWorldContext* WorldContext, void* URL, void* Pending, void* Error);

        bool Init();
        void Enable();
        void Disable();
    }
}