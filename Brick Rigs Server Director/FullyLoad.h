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
    class FullyLoad;
    inline FullyLoad* S_FullyLoad = nullptr; //Non-Inline causes link 2005

    class FullyLoad : public Hook<void, SDK::UPackage*>
    {
    public:

        static void __fastcall HookedFunction(SDK::UPackage* pkg)
        {
            std::cout << "Loading Package Fully: " << pkg->GetName() << std::endl;
            return S_FullyLoad->OriginalFunction(pkg);
        }
        
        FullyLoad() : Hook("\x40\x53\x48\x83\xEC\x40\x48\x8B\xD9\xE8\xC2\xBD\x00\x00\x84\xC0", "xxxxxxxxxxxxxxxx", HookedFunction, FAST) {}
    };
}