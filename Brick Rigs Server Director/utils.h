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
#include "offsets.h"

/*
* This file is meant to define utility macros for the SDK.
*/

/// <summary>
/// Function for static_cast
/// </summary>
template <typename T>
inline T Cast(void* obj)
{

	return static_cast<T>(obj);
}

/// <summary>
/// Gets the member of a object given an offset
/// </summary>
/// <typeparam name="T">member type</typeparam>
/// <param name="base">Address of the object.</param>
/// <param name="offset">offset of the member</param>
/// <returns>T</returns>
template<typename T>
T& GetMember(void* base, std::size_t offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<std::uint8_t*>(base) + offset);
}

/// <summary>
/// Sets the member of a object given an offset
/// </summary>
/// <typeparam name="T">member type</typeparam>
/// <param name="base">Address of the object.</param>
/// <param name="offset">offset of the member</param>
/// <param name="value">value to set the member</param>
/// <returns>T</returns>
template<typename T>
void SetMember(void* base, std::size_t offset, const T& value)
{
    *reinterpret_cast<T*>(reinterpret_cast<std::uint8_t*>(base) + offset) = value;
}

/// <summary>
/// Expression for whether or not the execution context is the game thread.
/// </summary>
#define IsInGameThread() (GetCurrentThreadId() == *reinterpret_cast<unsigned int*>(GGameThreadID))

/// <summary>
/// Gets the UClass from the sdk class. Will load bp classes as necessary
/// </summary>
/// <param name="cls">Class of the new object. Not the UClass. Ex: SDK::UBrickBorder </param>
/// <returns>A pointer to the UClass</returns>
#define GetUClass(cls) GetClassInternal<cls>(#cls)

/// <summary>
/// Gets the UClass from the sdk class. Will load bp classes as necessary
/// </summary>
/// <param name="cls">Class of the new object. Not the UClass. Ex: SDK::UBrickBorder </param>
/// <returns>A pointer to the UClass</returns>
#define UCLASS(cls) GetUClass(cls)

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