#pragma once
#include <windows.h>
#include <SDK.hpp>

namespace hooks
{
    // Use this to render primatives.Not currently used.The Signature should stay consistent unless UE version change
    namespace DrawTransition
    {
        inline bool enabled = false;
        inline bool initalized = false;
        inline const char* pattern = "\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x40\x80\xB9\x88\x00\x00\x00\x00\x48\x8B\xF2\x48\x8B\xF9\x0F\x85\x00\x00\x00\x00\x8B\x41\x08\x45\x33\xC0\xC1\xE8\x04\xA8\x01\x41\x8B\xC0";
        inline const char* mask = "xxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxx";
        inline uintptr_t DrawTransitionFunctionPointer = 0;

        using DrawTransition_t = void(__fastcall*)(SDK::UGameViewportClient* This, SDK::UCanvas* Canvas);
        inline DrawTransition_t OriginalDrawTransitionFunction = nullptr;

        void __fastcall HookedDrawTransitionFunction(SDK::UGameViewportClient* This, SDK::UCanvas* Canvas);

        bool Init();
        void Enable();
        void Disable();
    }
}
