/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     offsets.h											          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    7 July 2025                                                 */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/


#pragma once
#include <windows.h>
#include <libloaderapi.h>

/*
* This file is used to store various information regarding non-sdk functions/hooks so that when an update rolls around it is easier to update the information
* 
* Prefixes:
* F - Function offset
* V - VTable index
* H - Hook offset
* 
* Suffixes:
* _S = Signature
* _M = Mask
* 
* Hooks that use sigs/masks have suffixes as to not interfere with the existing naming conventions regarding singletons for the hooks.
*/

//Custom Assertion macro. takes an expression and a const char*. Aborts the game on expression failed.
#define ASSERT(exp, errmsg) \
    do { \
        if (!(exp)) { \
            char buf[512]; \
            snprintf(buf, sizeof(buf), \
                "Assertion failed!\n\nExpression: %s\nMessage: %s\nFile: %s\nLine: %d", \
                #exp, errmsg, __FILE__, __LINE__); \
            MessageBoxA(GetConsoleWindow(), buf, "BR-SDK Assertion FAILED", MB_OK); \
            std::abort(); \
        } \
    } while (0); \

#define BASE (unsigned long long )GetModuleHandle(NULL) //Add this to offsets. It is the base of BR: 0x140(IDA)

//define GlobalVarsOffsets

#define GGameThreadID BASE + 0x436DDA8

//Define function offsets

#define UBRICKBORDER_SYNCHRONIZEPROPERTIES "48 89 5C 24 ?? 48 89 74 24 ?? 55 57 41 54 41 56 41 57 48 8B EC 48 83 EC ?? 48 8B F1 E8 ?? ?? ?? ?? 45 33 E4 45 33 C0 BA ?? ?? ?? ??"
#define FFBrickChatMessageConstructor "48 89 5C 24 ?? 57 48 83 EC ?? 88 11 48 8B D9 48 83 C1 ?? 49 8B D0 33 FF E8 ?? ?? ?? ??"
#define FFFluUGCItemIdWrapperToString "48 8B C4 55 53 56 57 41 56 48 8B EC 48 83 EC ?? 48 8B 31 48 8B DA 48 85 F6"
#define FUniqueNetIdWrapper_ToString "48 89 5C 24 ?? 48 89 74 24 ?? 57 48 83 EC ?? 33 F6 48 8B F9 48 8B 49 ?? 48 8B DA"

//VTable funcs after this now dont need 8

//define hook offsets

#define HRestartPlayerAt "40 55 53 56 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 49 8B F8 48 89 54 24 ?? 48 8B C2 48 8B F1"

//define hook signatures/masks

#define BeginPlay_S "\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x8B\x89\x18\x01\x00\x00\x48\x85\xC9\x74\x1E\x48\x8B\x01"
#define BeginPlay_M "xxxxxxxxxxxxxxxxxxxxxxx"

#define DrawTransition_S "\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x40\x80\xB9\x88\x00\x00\x00\x00\x48\x8B\xF2\x48\x8B\xF9\x0F\x85\x00\x00\x00\x00\x8B\x41\x08\x45\x33\xC0\xC1\xE8\x04\xA8\x01\x41\x8B\xC0"
#define DrawTransition_M "xxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxx"

#define LoadMap_S "\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x80\x48\x81\xEC\x80\x01\x00\x00\x0F\x29\xBC\x24\x60\x01\x00\x00\x48\x8B\x05\x65\x77\xA7\x01"
#define LoadMap_M "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

#define OnPlayerJoined_S "\x48\x83\xEC\x48\x48\x89\x5C\x24\x58\x33\xDB\x48\x89\x74\x24\x68\x48\x89\x7C\x24\x40"
#define OnPlayerJoined_M "xxxxxxxxxxxxxxxxxxxx"

#define StartPlay_S "\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x8B\x89\x80\x02\x00\x00\x48\x8B\x01"
#define StartPlay_M "xxxxxxxxxxxxxxxxxxx"

#define OpenMenu_S "\x48\x89\x54\x24\x10\x55\x53\x56\x57\x41\x54\x41\x55\x48\x8D\x6C\x24\xD1\x48\x81\xEC"
#define OpenMenu_M "xxxxxxxxxxxxxxxxxxxx"


#define AddChatMessage_S "\x48\x89\x7C\x24\x18\x41\x56\x48\x83\xEC\x40\x48\x8B\xFA\x4C\x8B\xF1\xE8\x00\x00\x00\x00\x84\xC0\x0F\x84\x00\x00\x00\x00"
#define AddChatMessage_M "xxxxxxxxxxxxxxxxxx????xxxx????"
