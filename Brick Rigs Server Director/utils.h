/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     utils.h						                              */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    6 July 2025                                                 */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once

#include "spawnutils.h"
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
/// Use Spawn() for UWidget derieved objects 
/// </summary>
/// <param name="cls">The class of the new widget. Not the UClass. Ex: SDK::UWPB_PropertyContainer_C</param>
/// <returns>A pointer to the new widget</returns>
#define CreateWidget(cls) CreateWidgetInternal<cls>(cls::StaticClass(), #cls)

/// <summary>
/// Destroys an AActor using internal UE systems.
/// </summary>
/// <param name="actor">Pointer to the actor to be deleted</param>
/// <returns>None</returns>
#define SpawnActor(actor) actor->K2_DestroyActor()

/// <summary>
/// Spawns a new AActor using internal UE systems. Use when creating AActors
/// The actor will be created with the specified parent and spawn with its location and rotation
/// </summary>
/// <param name="cls">Class of the new actor. Not the UClass. Ex: SDK::ABrickCharacte </param>
/// <param name="parent">Pointer to the parent of the new actor. most not be null</param>
/// <returns>A pointer to the new actor</returns>
#define SpawnActor(cls, parent) SpawnActorInternal<cls>(parent, #cls)

/// <summary>
/// Spawns a new AActor using internal UE systems. Use when creating AActors
/// The actor will be created with the specified parent and spawn with the specified actors location and rotation
/// </summary>
/// <param name="cls">Class of the new actor. Not the UClass. Ex: SDK::ABrickCharacter </param>
/// <param name="parent">Pointer to the parent of the new actor./param>
/// <param name="actor">Pointer to the actor to get the location for the new actor</param>
/// <returns>A pointer to the new actor</returns>
#define SpawnActorAt(cls, parent, actor) SpawnActorInternal<cls>(actor->K2_GetActorLocation(), actor->K2_GetActorRotation(), parent, #cls)

/// <summary>
/// Spawns a new AActor using internal UE systems. Use when creating AActors
/// The actor will be created with the specified parent and spawn at the specified location
/// </summary>
/// <param name="cls">Class of the new actor. Not the UClass. Ex: SDK::ABrickCharacter </param>
/// <param name="parent">Pointer to the parent of the new actor./param>
/// <param name="vector">Location of the new actor</param>
/// <returns>A pointer to the new actor</returns>
#define SpawnActorVec(cls, parent, vector) SpawnActorInternal<cls>(vector, parent, #cls)

/// <summary>
/// Spawns a new AActor using internal UE systems. Use when creating AActors
/// The actor will be created with the specified parent and spawn with the specified location and rotation
/// </summary>
/// <param name="cls">Class of the new actor. Not the UClass. Ex: SDK::ABrickCharacter </param>
/// <param name="parent">Pointer to the parent of the new actor./param>
/// <param name="vector">Location of the new actor</param>
/// <param name="rotator">Rotation of the new actor</param>
/// <returns>A pointer to the new actor</returns>
#define SpawnActorLoc(cls, parent, vector, rotator) SpawnActorInternal<cls>(vector, rotator, parent, #cls)

/// <summary>
/// Spawns a new AActor using internal UE systems. Use when creating AActors
/// The actor will be created with the specified parent and spawn with the specified location and rotation
/// </summary>
/// <param name="cls">Class of the new actor. Not the UClass. Ex: SDK::ABrickCharacter </param>
/// <param name="parent">Pointer to the parent of the new actor./param>
/// <param name="transform">Transform of the new actor</param>
/// <returns>A pointer to the new actor</returns>
#define SpawnActorTrans(cls, parent, transform) SpawnActorInternal<cls>(transform, parent, #cls)



//TODO: Finish delayed actor spawn macros.

#define FinishSpawningActor(ptr, transform)