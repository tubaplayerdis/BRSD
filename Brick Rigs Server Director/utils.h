/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     utils.h						                              */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    7 July 2025                                                 */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once

#include "spawnutils.h"
#include "actorutils.h"
#include "Function.h"

/*
* This file is meant to define utility macros for the SDK.
*/

//TODO: Complete this macro:

/// <summary>
/// Gets the UClass from the sdk class. Will load bp classes as necessary
/// </summary>
/// <param name="cls">Class of the new object. Not the UClass. Ex: SDK::UBrickBorder </param>
/// <returns>A pointer to the UClass</returns>
#define GetUClass(cls)

/// <summary>
/// Spawns a new UObject using internal UE systems. Use when creating UObjects 
/// </summary>
/// <param name="cls">Class of the new object. Not the UClass. Ex: SDK::UBrickBorder </param>
/// <param name="out">A pointer to the outer object the new object should be created with</param>
/// <returns>A pointer to the new object</returns>
#define SpawnObject(cls, out) SpawnObjectInternal<cls>(out, #cls)

/// <summary>
/// Creates a new UUserWidget derived object using internal UE systems.
/// Use Spawn() for Non-UUserWidget derieved widgets
/// </summary>
/// <param name="cls">The class of the new widget. Not the UClass. Ex: SDK::UWPB_PropertyContainer_C</param>
/// <returns>A pointer to the new widget</returns>
#define CreateWidget(cls) CreateWidgetInternal<cls>(cls::StaticClass(), #cls)