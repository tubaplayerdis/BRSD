/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     LoadMap.h                                                   */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include "../../Utils/offsets.h"
#include <Windows.h>
#include <BR-SDK.hpp>
#include "../../Global/global.h"
#include "../../Modules/UI/watermark.h"


namespace hooks
{
    class LoadMap;
    inline LoadMap* S_LoadMap = nullptr; //Non-Inline causes link 2005

    class LoadMap : public Hook<char(SDK::UEngine*, SDK::FWorldContext*, void*, void*, void*)>
    {
    public:

        static char __fastcall HookedFunction(SDK::UEngine* This, SDK::FWorldContext* WorldContext, void* URL, void* Pending, void* Error)
        {
            char ret = S_LoadMap->OriginalFunction(This, WorldContext, URL, Pending, Error);
            Sleep(20);
            global::mapLevelName = SDK::UGameplayStatics::GetCurrentLevelName(World(), true).ToString();
            if (global::mapLevelName == "MainMenu") watermark::ShowWaterMark();
            return ret;
        }

        LoadMap() : Hook(UENGINE_LOADMAP, HookedFunction) {}
    };
}