/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     LoadPackage.h                                               */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <windows.h>
#include <SDK.hpp>
#include "global.h"
#include "Hook.h"

namespace hooks
{
    class LoadPackage;
    inline LoadPackage* S_LoadPackage = nullptr; //Non-Inline causes link 2005

    class LoadPackage : public Hook<SDK::UPackage*, SDK::UPackage*, const wchar_t*, unsigned int, void*, void*>
    {
    public:

        static SDK::UPackage* __fastcall HookedFunction(SDK::UPackage* pkg, const wchar_t* pkgName, unsigned int flags , void* smth1, void* smth2)
        {
            std::wcout << L"Package-Name: " << pkgName << std::endl;
            return S_LoadPackage->OriginalFunction(pkg, pkgName, flags, smth1, smth2);
        }
        //                      48  8B  C4  48  89  48  08  53  57  48  83  EC  58  48  89  70  18  48  8D  48  C8
        LoadPackage() : Hook("\x48\x8B\xC4\x48\x89\x48\x08\x53\x57\x48\x83\xEC\x58\x47\x89\x70\x18\x48\x8D\x48\xC8", "xxxxxxxxxxxxxxxxxxxxx", HookedFunction, false) {}
    };
}