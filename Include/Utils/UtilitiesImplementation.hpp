/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     UtilitiesImplementation.hpp				      */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    14 July 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include "../SDK/Basic.hpp"
#include "../SDK/BrickRigs_classes.hpp"
#include "../SDK/UMG_classes.hpp"
#include "../../Include/Utils/Offsets.hpp"
#include "../../Include/Utils/GameFunctions.hpp"

/// Loads a blueprint class from the SDK
/// @param classname String representation of the class the SDK:: will be filtered out if present, IE: SDK::ABP_CarElevator_C, or ABP_CarElevator_C.
void AttemptLoadClass(const char* classname);

template<typename T>
SDK::UClass* GetClassInternal(const char* clsobjname)
{
	SDK::UClass* objcls = T::StaticClass();
	if (!objcls) AttemptLoadClass(clsobjname);
	objcls = T::StaticClass();
	ASSERT(objcls != nullptr, std::string("Class: ") + std::string(clsobjname) + std::string(" Could not be loaded!"));
	return objcls;
}

//Class safe version of SDK::UGameplayStatics::SpawnObject. Use the SpawnObject() macro instead of this function.
template<typename T>
T* SpawnObjectInternal(SDK::UObject* outerobj, const char* objclsname)
{
	SDK::UClass* objcls = GetClassInternal<T>(objclsname);
	return static_cast<T*>(SDK::UGameplayStatics::SpawnObject(objcls, outerobj));
}

template<typename T>
T* CreateWidgetInternal(SDK::TSubclassOf<SDK::UUserWidget> UserWidgetClass, const char* WidgetClassName)
{

	if (UserWidgetClass == nullptr) UserWidgetClass = GetClassInternal<T>(WidgetClassName);
	return static_cast<T*>(CallGameFunction<SDK::UUserWidget*, SDK::UWorld*, SDK::TSubclassOf<SDK::UUserWidget>, SDK::FName>(F_CREATE_WIDGET, SDK::UWorld::GetWorld(), UserWidgetClass, SDK::FName()));
}

template<typename T>
T* SpawnActorInternal(SDK::AActor* outeract, const char* objclsname)
{
	ASSERT(outeract == nullptr, "Actors only spawned in refrence to another actor must not be null!");
	SDK::FTransform transform = outeract->GetTransform();
	SDK::UClass* objcls = GetClassInternal<T>(objclsname);
	SDK::ESpawnActorCollisionHandlingMethod method = SDK::ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SDK::AActor* act = SDK::UGameplayStatics::BeginDeferredActorSpawnFromClass(SDK::UWorld::GetWorld(), objcls, transform, method, outeract);
	return static_cast<T*>(SDK::UGameplayStatics::FinishSpawningActor(act, transform));
}

template<typename T>
T* SpawnActorInternal(SDK::FTransform transform, SDK::AActor* outeract, const char* objclsname, bool deferred)
{
	SDK::UClass* objcls = GetClassInternal<T>(objclsname);
	SDK::ESpawnActorCollisionHandlingMethod method = SDK::ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SDK::AActor* act = SDK::UGameplayStatics::BeginDeferredActorSpawnFromClass(SDK::UWorld::GetWorld(), objcls, transform, method, outeract);
	SDK::UGameplayStatics::FinishSpawningActor(act, transform);
	return static_cast<T*>(act);
}

template<typename T>
T* SpawnActorInternal(SDK::FVector position, SDK::AActor* outeract, const char* objclsname)
{
	SDK::FRotator rotator = SDK::FRotator();
	if (!outeract)
	{
		rotator = SDK::FRotator(0, 0, 0);
	}
	else rotator = outeract->K2_GetActorRotation();
	SDK::FTransform transform = SDK::FTransform();
	transform.Translation = position;
	transform.Scale3D = SDK::FVector(1, 1, 1);
	SDK::FQuat formed = SDK::FQuat();
	SDK::FQuat* ptr = CallGameFunction<SDK::FQuat*, SDK::FRotator*, SDK::FQuat*>(F_QUATERNION, &rotator, &formed);
	formed = *ptr;
	transform.Rotation = formed;
	SDK::UClass* objcls = GetClassInternal<T>(objclsname);
	SDK::ESpawnActorCollisionHandlingMethod method = SDK::ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SDK::AActor* act = SDK::UGameplayStatics::BeginDeferredActorSpawnFromClass(SDK::UWorld::GetWorld(), objcls, transform, method, outeract);
	SDK::UGameplayStatics::FinishSpawningActor(act, transform);
	return static_cast<T*>(act);
}

template<typename T>
T* SpawnActorInternal(SDK::FVector position, SDK::FRotator rotation, SDK::AActor* outeract, const char* objclsname)
{
	SDK::FTransform transform = SDK::FTransform();
	transform.Translation = position;
	transform.Scale3D = SDK::FVector(1, 1, 1);
	SDK::FQuat formed = SDK::FQuat();
	CallGameFunction<SDK::FQuat*, SDK::FRotator*, SDK::FQuat*>(F_QUATERNION, &rotation, &formed);
	transform.Rotation = formed;
	SDK::UClass* objcls = GetClassInternal<T>(objclsname);
	SDK::ESpawnActorCollisionHandlingMethod method = SDK::ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SDK::AActor* act = SDK::UGameplayStatics::BeginDeferredActorSpawnFromClass(SDK::UWorld::GetWorld(), objcls, transform, method, outeract);
	SDK::UGameplayStatics::FinishSpawningActor(act, transform);
	return static_cast<T*>(act);
}