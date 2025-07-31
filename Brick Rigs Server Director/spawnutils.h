/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     spawnutils.h					                              */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    6 July 2025                                                 */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include "Function.h"
#include "offsets.h"
#include <windows.h>
#include <tlhelp32.h>
#include <SDK.hpp>

//Enabling my laziness
#define _itor(num) int i = 0; i < num; i++

namespace _spawnutils
{
	#pragma region helpers

	//Gets the FPlatformFileManager
	inline void* GetPlatformFile()
	{
		return CallGameFunction<void*, void*>(FGetPlatformFile, CallGameFunction<void*>(FGetPlatformFileManager));
	}

	//Sets the path of a FSoftObjectPath
	inline void SetPath(SDK::FakeSoftObjectPtr::FSoftObjectPath* This, SDK::FName pathname)
	{
		return CallGameFunction<void, SDK::FakeSoftObjectPtr::FSoftObjectPath*, SDK::FName>(FSetPath, This, pathname);
	}

	//Gets every asset file in the virtual file system
	inline SDK::TArray<SDK::FString> GetVFSFiles()
	{
		SDK::FString gamepath = SDK::UBlueprintPathsLibrary::ProjectContentDir();
		SDK::TArray<SDK::FString> ret;
		CallGameFunction<void, void*, SDK::TArray<SDK::FString>*, const wchar_t*, const wchar_t*>(FFindFilesRecursively, GetPlatformFile(), &ret, gamepath.CStr(), L".uasset");
		return ret;
	}

	//Basic std::string to std::wstring
	inline std::wstring _to_wstring(const std::string& str)
	{
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
		std::wstring wstr(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
		wstr.pop_back(); // remove null terminator
		return wstr;
	}

	//Finds the asset path of the class in string form
	inline std::wstring FindClassAssetPath(const char* classname)
	{
		std::wstring universal = _to_wstring(classname);
		universal = universal.substr(universal.find_first_of('B'));//remove the ABP, UBP, UWBP.
		//std::wcout << "Attempting Load: " << universal << std::endl;
		std::wstring lookfor = universal;
		lookfor.pop_back();
		lookfor.pop_back();
		lookfor.append(L".");//Remove the _C and add a dot so that we know when the name ends.
		std::wstring res = std::wstring(L"NONE");
		SDK::TArray<SDK::FString> files = GetVFSFiles();
		for (int i = 0; i < files.Num(); i++)
		{
			if (files[i].ToWString().find(std::wstring(lookfor)) != std::string::npos)
			{
				//../../../BrickRigs/Content/BrickRigs/UI/Properties/WBP_PropertiesPanel.uasset
				std::wstring original = files[i].ToWString();
				const std::wstring target = L"BrickRigs/";

				// Find the first occurrence of "BrickRigs/"
				size_t firstBrickRigs = original.find(target);
				if (firstBrickRigs == std::wstring::npos) {
					break;
				}

				// Find the next "BrickRigs/" after the first one (skipping "BrickRigs/Content/")
				size_t secondBrickRigs = original.find(target, firstBrickRigs + target.length());
				if (secondBrickRigs == std::wstring::npos) {
					break;
				}
				// Extract from the second "BrickRigs/"
				std::wstring result = original.substr(secondBrickRigs);

				// Remove the ".uasset" extension
				size_t dotPos = result.rfind('.');
				if (dotPos != std::wstring::npos) {
					result = result.substr(0, dotPos);
				}

				//Find the package name. Everything after the last slash
				size_t slashpos = result.find_last_of('/');
				std::wstring packname = L"";
				if (slashpos != std::wstring::npos)
				{
					packname = result.substr(slashpos + 1);
				}

				result = L"/Game/" + result;

				result.append(L".");
				result.append(packname);
				result.append(L"_C");

				//std::wcout << result << std::endl;

				res = result;
				break;
			}
		}

		if (res == L"NONE") { std::cout << "Failed to Find Class!" << std::endl; return L"NONE"; }

		return res.c_str();
	}

	inline void* GetStreamableManager()
	{
		if ((void*)SDK::UBrickAssetManager::Get()->StreamableManager == nullptr) std::cout << "dude" << std::endl;
		return (void*)SDK::UBrickAssetManager::Get()->StreamableManager;
	}

	struct FSHFiller
	{
		uint8_t padding[0x10];
	};

	struct FStreamableHandle : FSHFiller
	{
		bool bLoadCompleted;
		bool bReleased;
		bool bCanceled;
		bool bStalled;
		bool bReleaseWhenLoaded;
		bool bIsCombinedHandle;
		uint8_t pad[50];
		SDK::FString DebugName;
		int priority;
		int StreamablesLoading;
		int CompletedChildCount;
		int CanceledChildCount;
		uint8_t pad1[48];
		void* OwningManager;
	};

	static_assert(sizeof(FStreamableHandle) == 0xA0);

	//Mimics TSharedPtr
	struct SharedPtr
	{
		FStreamableHandle* ptr;
		uint8_t pad[0x8];
	};

	inline void RequestAsyncLoad(SDK::FakeSoftObjectPtr::FSoftObjectPath* path)
	{
		SDK::UGunBrick* BrickHandeler = static_cast<SDK::UGunBrick*>(SDK::UGameplayStatics::SpawnObject(SDK::UGunBrick::StaticClass(), SDK::UWorld::GetWorld()));
		SDK::TDelegate<void(void)> dele = SDK::TDelegate<void(void)>();
		uintptr_t fnAddress = FMarkBrickBurnt;//MarkBrickBurnt
		// Manually build the 16-byte function pointer representation
		uint64_t funcBlob[2] = { fnAddress, 0 };
		SDK::TDelegate<void(void)>* delenew = CallGameFunction<SDK::TDelegate<void(void)>*, SDK::TDelegate<void(void)>*, SDK::UGunBrick*, uint64_t*>(FCreateUObject, &dele, BrickHandeler, funcBlob);
		SharedPtr ptr{};
		ptr.ptr = nullptr;
		UC::FString str = UC::FString(L"LoadAssetList");
		SharedPtr* ptrret = CallGameFunction<SharedPtr*, void*, SharedPtr*, const SDK::FakeSoftObjectPtr::FSoftObjectPath*, SDK::TDelegate<void __cdecl(void)>*, int, bool, bool, SDK::FString*>(FRequestAsyncLoad, GetStreamableManager(), &ptr, path, delenew, 0, false, false, &str);
		int max = 0;
		while (max <= 10)
		{
			Sleep(50);
			if (BrickHandeler->IsBrickBurnt()) break;
			max++;
			std::cout << "Brick was not burnt" << std::endl;
		}
		CallGameFunction<__int64, void*, float, bool>(FWaitUntilComplete, ptrret->ptr, 0.0, 0);//Safe to call and finalize the load.
		//If this becomes problematic or in need of change maybe try to hook FEngineLoop::Tick and be able to send in lambdas. that should run code on the main thread.
	}

	/// <summary>
	/// Loads a blueprint class from the SDK
	/// </summary>
	/// <param name="classname">String representation of the class the SDK:: will be filtered out if present, IE: SDK::ABP_CarElevator_C, or ABP_CarElevator_C.</param>
	inline void AttemptLoadClass(const char* classname)
	{
		SDK::FString ClassPath = SDK::FString(FindClassAssetPath(classname).c_str());
		SDK::TSoftClassPtr<SDK::UClass> ptr = SDK::TSoftClassPtr<SDK::UClass>();
		const SDK::FName path = SDK::UKismetStringLibrary::Conv_StringToName(ClassPath);
		SetPath(&ptr.ObjectID, path);
		if (GetCurrentThreadId() == *reinterpret_cast<unsigned int*>(GGameThreadID))//IsInGameThreadMacro
		{
			//Load the class using the blocking thread.
			CallGameFunction<SDK::UObject*, SDK::FSoftObjectPtr*>(FLoadSynchronous, &ptr);
			return;
		}
		RequestAsyncLoad(&ptr.ObjectID);
		return;
	}


	#ifdef _DEBUG
	inline void LoadEveryClassPossible()
	{
		SDK::TArray<SDK::FString> files = GetVFSFiles();
		for (int i = 0; i < files.Num(); i++)
		{
			//../../../BrickRigs/Content/BrickRigs/UI/Properties/WBP_PropertiesPanel.uasset
			std::wstring original = files[i].ToWString();
			const std::wstring target = L"BrickRigs/";

			// Find the first occurrence of "BrickRigs/"
			size_t firstBrickRigs = original.find(target);
			if (firstBrickRigs == std::wstring::npos) {
				break;
			}

			// Find the next "BrickRigs/" after the first one (skipping "BrickRigs/Content/")
			size_t secondBrickRigs = original.find(target, firstBrickRigs + target.length());
			if (secondBrickRigs == std::wstring::npos) {
				break;
			}
			// Extract from the second "BrickRigs/"
			std::wstring result = original.substr(secondBrickRigs);

			// Remove the ".uasset" extension
			size_t dotPos = result.rfind('.');
			if (dotPos != std::wstring::npos) {
				result = result.substr(0, dotPos);
			}

			size_t clsnamepos = result.rfind(L"/");
			std::wstring classname = L"";
			if (dotPos != std::wstring::npos) {
				classname = result.substr(clsnamepos + 1);
				classname.append(L"_C");
			}

			result = L"/Game/" + result;

			result.append(L".");
			result.append(classname);

			std::wcout << L"attempting load on: " << result << std::endl;
			
			SDK::TSoftClassPtr<SDK::UClass> ptr = SDK::TSoftClassPtr<SDK::UClass>();
			const SDK::FName path = SDK::UKismetStringLibrary::Conv_StringToName(SDK::FString(result.c_str()));//FString is volatile and wrong. only use as const for final step moving on.
			SetPath(&ptr.ObjectID, path);
			if (GetCurrentThreadId() == *reinterpret_cast<unsigned int*>(GGameThreadID))//IsInGameThreadMacro
			{
				//Load the class using the blocking thread.
				CallGameFunction<SDK::UObject*, SDK::FSoftObjectPtr*>(FLoadSynchronous, &ptr);
				return;
			}
			RequestAsyncLoad(&ptr.ObjectID);
		}
	}
	#endif // _DEBUG

	#pragma endregion
}

template<typename T>
inline SDK::UClass* GetClassInternal(const char* clsobjname)
{
	SDK::UClass* objcls = T::StaticClass();
	if (!objcls) _spawnutils::AttemptLoadClass(clsobjname);
	objcls = T::StaticClass();
	return objcls;
}

//Example parameter UBP_DamageType_FuelExplosion_C
template<typename T>
inline T* SpawnObjectInternal(SDK::UObject* outerobj, const char* objclsname)
{
	SDK::UClass* objcls = GetClassInternal<T>(objclsname);
	return static_cast<T*>(SDK::UGameplayStatics::SpawnObject(objcls, outerobj));
}

template<typename T>
inline T* CreateWidgetInternal(SDK::TSubclassOf<SDK::UUserWidget> UserWidgetClass, const char* WidgetClassName)
{

	if (UserWidgetClass == nullptr) UserWidgetClass = GetClassInternal<T>(WidgetClassName);
	return static_cast<T*>(CallGameFunction<SDK::UUserWidget*, SDK::UWorld*, SDK::TSubclassOf<SDK::UUserWidget>, SDK::FName>(FCreateWidget, SDK::UWorld::GetWorld(), UserWidgetClass, SDK::FName()));
}

template<typename T>
inline T* SpawnActorInternal(SDK::AActor* outeract, const char* objclsname)
{
	ASSERT(outeract == nullptr, "Actors only spawned in refrence to another actor must not be null!");
	SDK::FTransform transform = outeract->GetTransform();
	SDK::UClass* objcls = GetClassInternal<T>(objclsname);
	SDK::ESpawnActorCollisionHandlingMethod method = SDK::ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SDK::AActor* act = SDK::UGameplayStatics::BeginDeferredActorSpawnFromClass(SDK::UWorld::GetWorld(), objcls, transform, method, outeract);
	return static_cast<T*>(SDK::UGameplayStatics::FinishSpawningActor(act, transform));
}

template<typename T>
inline T* SpawnActorInternal(SDK::FTransform transform, SDK::AActor* outeract, const char* objclsname, bool deferred)
{
	SDK::UClass* objcls = GetClassInternal<T>(objclsname);
	SDK::ESpawnActorCollisionHandlingMethod method = SDK::ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SDK::AActor* act = SDK::UGameplayStatics::BeginDeferredActorSpawnFromClass(SDK::UWorld::GetWorld(), objcls, transform, method, outeract);
	SDK::UGameplayStatics::FinishSpawningActor(act, transform);
	return static_cast<T*>(act);
}

template<typename T>
inline T* SpawnActorInternal(SDK::FVector position, SDK::AActor* outeract, const char* objclsname)
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
	SDK::FQuat* ptr = CallGameFunction<SDK::FQuat*, SDK::FRotator*, SDK::FQuat*>(FQuaternion, &rotator, &formed);
	formed = *ptr;
	transform.Rotation = formed;
	SDK::UClass* objcls = GetClassInternal<T>(objclsname);
	SDK::ESpawnActorCollisionHandlingMethod method = SDK::ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SDK::AActor* act = SDK::UGameplayStatics::BeginDeferredActorSpawnFromClass(SDK::UWorld::GetWorld(), objcls, transform, method, outeract);
	SDK::UGameplayStatics::FinishSpawningActor(act, transform);
	return static_cast<T*>(act);
}

template<typename T>
inline T* SpawnActorInternal(SDK::FVector position, SDK::FRotator rotation, SDK::AActor* outeract, const char* objclsname)
{
	SDK::FTransform transform = SDK::FTransform();
	transform.Translation = position;
	transform.Scale3D = SDK::FVector(1, 1, 1);
	SDK::FQuat formed = SDK::FQuat();
	formed = &CallGameFunction<SDK::FQuat*, SDK::FRotator*, SDK::FQuat*>(FQuaternion, &rotation, &formed);
	transform.Rotation = formed;
	SDK::UClass* objcls = GetClassInternal<T>(objclsname);
	SDK::ESpawnActorCollisionHandlingMethod method = SDK::ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SDK::AActor* act = SDK::UGameplayStatics::BeginDeferredActorSpawnFromClass(SDK::UWorld::GetWorld(), objcls, transform, method, outeract);
	SDK::UGameplayStatics::FinishSpawningActor(act, transform);
	return static_cast<T*>(act);
}