/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     FSlateColor.cpp                                             */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "FSlateColor.h"
#include "FLinearColor.h"
#include "global.h"

void FSlateColor::FSlateColorConstructor(SDK::FSlateColor* This, SDK::FLinearColor* InColor)
{
    const char* pattern =
        "\x0F\x10\x02\x33\xC0\x0F\x11\x01"
        "\x48\x89\x41\x18\x48\x89\x41\x20"
        "\x48\x8B\xC1\xC6\x41\x10\x00\xC3";
    const char* mask = "xxxxxxxxxxxxxxxxxxxxxxxx";

    uintptr_t FSlateColorConstructorPointer = FindPattern(pattern, mask, GetModuleBaseN(), GetModuleSizeN());

    using FSlateColorConstructorFn = void(__fastcall*)(SDK::FSlateColor* Thiss, SDK::FLinearColor* InColorr);
    FSlateColorConstructorFn OnFSlateColorConstructor = reinterpret_cast<FSlateColorConstructorFn>(FSlateColorConstructorPointer);

    OnFSlateColorConstructor(This, InColor);
}

SDK::FSlateColor FSlateColor::CreateSlateColor(float R, float G, float B, float A)
{
    SDK::FSlateColor ret;
    SDK::FLinearColor col = FLinearColor::CreateLinearColor(R, G, B, A);
    FSlateColorConstructor(&ret, &col);
    return ret;
}
