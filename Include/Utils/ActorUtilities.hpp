/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     ActorUtilities.hpp						                  */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    14 July 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once

#include "UtilitiesImplementation.hpp"


/// Destroys an AActor using internal UE systems.
/// @param actor Pointer to the actor to be deleted
#define DestroyActor(actor) actor->K2_DestroyActor()

/// Spawns a new AActor using internal UE systems. Use when creating AActors
/// The actor will be created with the specified parent and spawn with its location and rotation
/// If a parent is not specified, its location and rotation will be 0,0,0
/// @param cls Class of the new actor. Not the UClass. Ex: SDK::ABrickCharacter
/// @param parent Pointer to the parent of the new actor.
/// @return A pointer to the new actor
#define SpawnActor(cls, parent) SpawnActorInternal<cls>(parent, #cls)

/// Spawns a new AActor using internal UE systems. Use when creating AActors
/// The actor will be created with the specified parent and spawn with the specified actors location and rotation
/// @param cls Class of the new actor. Not the UClass. Ex: SDK::ABrickCharacter
/// @param parent Pointer to the parent of the new actor.
/// @param actor Pointer to the actor to get the location for the new actor
/// @return A pointer to the new actor
#define SpawnActorAt(cls, parent, actor) SpawnActorInternal<cls>(actor->K2_GetActorLocation(), actor->K2_GetActorRotation(), parent, #cls)

/// Spawns a new AActor using internal UE systems. Use when creating AActors
/// The actor will be created with the specified parent and spawn at the specified location
/// @param cls Class of the new actor. Not the UClass. Ex: SDK::ABrickCharacter
/// @param parent Pointer to the parent of the new actor.
/// @param vector Location of the new actor
/// @return A pointer to the new actor
#define SpawnActorVec(cls, parent, vector) SpawnActorInternal<cls>(vector, parent, #cls)

/// Spawns a new AActor using internal UE systems. Use when creating AActors
/// The actor will be created with the specified parent and spawn with the specified location and rotation
/// @param cls Class of the new actor. Not the UClass. Ex: SDK::ABrickCharacter
/// @param parent Pointer to the parent of the new actor.
/// @param vector Location of the new actor
/// @param rotator Rotation of the new actor
/// @return A pointer to the new actor
#define SpawnActorLoc(cls, parent, vector, rotator) SpawnActorInternal<cls>(vector, rotator, parent, #cls)

/// Spawns a new AActor using internal UE systems. Use when creating AActors
/// The actor will be created with the specified parent and spawn with the specified location and rotation
/// @param cls Class of the new actor. Not the UClass. Ex: SDK::ABrickCharacter
/// @param parent Pointer to the parent of the new actor.
/// @param transform Transform of the new actor
/// @return A pointer to the new actor
#define SpawnActorTrans(cls, parent, transform) SpawnActorInternal<cls>(transform, parent, #cls, false)

/// Spawns a new AActor using internal UE systems and be deferred until FinishSpawningActor has been called. Use when creating AActors
/// The actor will be created with the specified parent and spawn with the specified location and rotation
/// @param cls Class of the new actor. Not the UClass. Ex: SDK::ABrickCharacter
/// @param parent Pointer to the parent of the new actor.
/// @param transform Transform of the new actor
/// @return A pointer to the new actor
#define DeferredSpawnActorTrans(cls, parent, transform) SpawnActorInternal<cls>(transform, parent, #cls, true)

/// Finishes spawning a deferred actor created with DeferredSpawnActorTrans
/// The actor will be created with the specified transform
/// @param actor Pointer to the actor to finish spawning
/// @param transform Transform of the new actor
/// @return A pointer to the new actor
#define FinishSpawningDeferredActor(actor, transform) SDK::UGameplayStatics::FinishSpawningActor(actor, transform);