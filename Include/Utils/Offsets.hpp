/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     Offsets.hpp										          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    14 July 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/


#pragma once
#include <windows.h>
#include <libloaderapi.h>

//Version of Brick Rigs the SDK is mapped to.

#define MAPPED_SDK_VERSION_RELEASE 1
#define MAPPED_SDK_VERSION_MAJOR 8
#define MAPPED_SDK_VERSION_MINOR 4

/*
* This file is used to store various information regarding non-sdk functions/hooks so that when an update rolls around it is easier to update the information
*
* Prefixes:
* F_ - Function offset
* V_ - VTable index
* H_ - Hook offset
* G_ - Global variable offset
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

#define BASE (unsigned long long )GetModuleHandle(NULL) //Add this to offsets. It is the module base of Brick Rigs and look like 0x14 in IDA

//Global variable offsets
#define G_GAME_THREAD_ID (BASE + 0x436DDA8)

//Function offsets
#define F_CREATE_WIDGET "48 83 EC ?? 48 85 C9 74 ?? 48 89 5C 24 ?? E8 ?? ?? ?? ?? 48 8B D8 48 85 C0"
#define F_SET_PATH "48 89 5C 24 ?? 48 89 7C 24 ?? 48 89 54 24 ?? 55 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ??"
#define F_GET_PLATFORM_FILE "40 53 48 83 EC ?? 48 8B 01 48 8B D9 48 85 C0 75 ?? E8 ?? ?? ?? ?? 48 89 03"
#define F_GET_PLATFORM_FILE_MANAGER (BASE + 0x0F19360) //Loads a singleton object.
#define F_FIND_FILES_RECURSIVELY (BASE + 0x0EEF8C0) //Matches very well with FindFiles. ("48 89 5C 24 ?? 48 89 6C 24 ?? 48 89 74 24 ?? 57 48 83 EC ?? 48 8B F9 48 89 4C 24 ??")
#define F_REQUEST_ASYNC_LOAD "4C 8B DC 49 89 5B ?? 49 89 6B ?? 49 89 73 ?? 49 89 7B ?? 41 56 48 81 EC ?? ?? ?? ?? 49 8B 00"
#define F_WAIT_UNTIL_COMPLETE "44 88 44 24 ?? 48 89 4C 24 ?? 55 56 48 8D 6C 24 ?? 48 81 EC ?? ?? ?? ?? 80 79 ?? ??"
#define F_QUATERNION "48 83 EC ?? 0F 28 2D ?? ?? ?? ?? F3 0F 10 51 ?? 0F 29 74 24 ?? 0F 29 7C 24 ??"
#define F_LOAD_SYNCHRONOUS "48 89 5C 24 ?? 48 89 6C 24 ?? 48 89 74 24 ?? 57 48 83 EC ?? 48 8B F9 E8 ?? ?? ?? ?? 33 ED 48 8B F0 48 85 C0 75 ?? 48 39 6F ??"
#define F_UNINITIALIZE_EDITOR_OBJECT "40 53 48 83 EC ?? 48 8B 81 ?? ?? ?? ?? 48 8B D9 48 85 C0 74 ?? 48 83 38 ?? 74 ?? B2 ?? E8 ?? ?? ?? ??"
#define F_CREATE_UOBJECT (BASE + 0x07DD430) //Actual function below (Matches with other functions too well): ("48 89 5C 24 ?? 48 89 74 24 ?? 57 48 83 EC ?? 33 C0 48 8B F2 48 89 01 48 8B F9 89 41 ?? BA ?? ?? ?? ?? 41 0F 10 00 45 33 C0")
//TDelegate<void __cdecl(void),FDefaultDelegateUserPolicy> *__fastcall TDelegate<void (void),FDefaultDelegateUserPolicy>::CreateUObject<UGunBrick,>(TDelegate<void __cdecl(void),FDefaultDelegateUserPolicy> *result,UGunBrick *InUserObject,__int128 *InFunc)
//This functionality should be disabled in BR-SDK unless fluppi provides the pdb for 1.9 +