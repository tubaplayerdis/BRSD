/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     RequestAsyncLoad.h                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    7 July 2025                                                 */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include "offsets.h"
#include <windows.h>
#include <SDK.hpp>
#include "psettings.h"
#include "Hook.h"

struct FCGObject
{
    uint8_t pada[0x10];
};

struct FStreamableManager : FCGObject
{
    uint8_t pad1[0x50];
    uint8_t pad2[0x50];
    uint8_t pad3[0x10];
    uint8_t pad4[0x10];
    bool forcesync;
    uint8_t pad5[7];
    uint8_t pad6[0x10];
};

static_assert(sizeof(FStreamableManager) == 232);

namespace hooks
{
    class RequestAsyncLoad;
    inline RequestAsyncLoad* S_RequestAsyncLoad = nullptr; //Non-Inline causes link 2005

    //Hook is currently not used
    class RequestAsyncLoad : public Hook<void*, FStreamableManager*, void*, void*, void*, int, bool, bool, void*>
    {
    public:

        static void* __fastcall HookedFunction(FStreamableManager* This, void* v1, void* v2, void* v3, int i, bool b1, bool b2, void* v4)
        {
            std::cout << "async load was called!" << std::endl;
            This->forcesync = false;
            return S_RequestAsyncLoad->OriginalFunction(This, v1, v2, v3,i, b1, b2, v4);
        }

        RequestAsyncLoad() : Hook(HOnRequestAsyncLoad, HookedFunction) {}
    };
}