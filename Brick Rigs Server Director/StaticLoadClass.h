/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     StaticLoadClass.h                                           */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <Windows.h>
#include <SDK.hpp>
#include "global.h"
#include "Hook.h"

namespace hooks
{
    class StaticLoadClass;
    inline StaticLoadClass* S_StaticLoadClass = nullptr; //Non-Inline causes link 2005

    class StaticLoadClass : public Hook<SDK::UClass*, SDK::UClass*, SDK::UObject*, const wchar_t*, const wchar_t*, unsigned int, SDK::UPackageMap*>
    {
    public:

        static SDK::UClass* __fastcall HookedFunction(SDK::UClass* BaseClass, SDK::UObject* InOuter, const wchar_t* InName, const wchar_t* Filename, unsigned int LoadFlags, SDK::UPackageMap* Sandbox)
        {
            std::wcout << L"In-Name: " << InName << std::endl;
            return S_StaticLoadClass->OriginalFunction(BaseClass, InOuter, InName, Filename, LoadFlags, Sandbox);
        }

        StaticLoadClass() : Hook("\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\xC8\x48\x81\xEC\x38\x01\x00\x00\x48\x8B\x05\x40\x1D\x0A\x03", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", HookedFunction) {}
    };
}