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

#define BASE (unsigned long long )GetModuleHandle(NULL) //Add this to offsets. It is the base of BR: 0x140(IDA)

//Define function offsets

#define FCreateWidget BASE + 0x0CC6140
#define FSynchronizeProperties_UBB BASE + 0x0DE8030
#define FSynchronizeProperties_UBSB BASE + 0x0DE9250
#define FUpdateInputMode BASE + 0x0DEEB70
#define FOpenMenu BASE + 0x0D7AD00
#define FGetModHookSubsystem BASE + 0x0D20EE0
#define FFBrickChatMessageConstructor BASE + 0x0D0EA10
#define FFindOrLoadClass BASE + 0x0C33890
#define FLoadPackage BASE + 0x121D710
#define FLoadPackageAsync BASE + 0x10A8AA0
#define FDoesPackageExist BASE + 0x105F8A0

//define VTable offsets. 8 is used after the number IDA gives

#define VInitalize 0x2D0 / 8
#define VisServer 0x378 / 8

//define hook offsets

#define HOnClicked BASE + 0x0D76780

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