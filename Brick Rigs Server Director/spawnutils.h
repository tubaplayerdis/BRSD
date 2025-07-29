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
	inline SDK::TDelegate<void __cdecl(void)> delasync = SDK::TDelegate<void __cdecl(void)>();

	#pragma region helpers

	inline int GetObjSerialNumber(int index)
	{
		return SDK::UObject::GObjects->SDGetByIndex(index);
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

	struct mockFStreamableHandle : FSHFiller
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

	static_assert(sizeof(mockFStreamableHandle) == 0xA0);

	struct falseSharedPtr
	{
		mockFStreamableHandle* ptr;
		uint8_t pad[0x8];
	};

	inline void RequestAsyncLoad(SDK::FakeSoftObjectPtr::FSoftObjectPath* path)
	{
		delasync = SDK::TDelegate<void __cdecl(void)>();//Create new fuckass delegate becuase it most likey got deleted
		falseSharedPtr ptr{};
		ptr.ptr = nullptr;
		UC::FString str = UC::FString(L"LoadAssetList");
		falseSharedPtr* ptrret = CallGameFunction<falseSharedPtr*, void*, falseSharedPtr*, const SDK::FakeSoftObjectPtr::FSoftObjectPath*, SDK::TDelegate<void __cdecl(void)>*, int, bool, bool, SDK::FString*>(FRequestAsyncLoad, GetStreamableManager(), &ptr, path, &delasync, 0, false, false, &str);

		//Sleep(10);//I have no other ideas.
		if(!ptrret->ptr->bLoadCompleted) CallGameFunction<__int64, void*, float, bool>(FWaitUntilComplete, ptrret->ptr, 1000.0, 0);
		std::cout << "passed wait with: " << ptrret->ptr->bLoadCompleted << std::endl;
		//If this becomes problematic or in need of change maybe try to hook FEngineLoop::Tick and be able to send in lambdas. that should run code on the main thread.
	}
 
	inline void RequestAsyncLoad_D(SDK::FakeSoftObjectPtr::FSoftObjectPath* path, int num)
	{
		falseSharedPtr ptr{};
		ptr.ptr = nullptr;
		UC::FString str = UC::FString(L"LoadAssetList");
		//Returns a shared pointer pointer to the handle
		falseSharedPtr* ptrret = CallGameFunction<falseSharedPtr*, void*, falseSharedPtr*, const SDK::FakeSoftObjectPtr::FSoftObjectPath*, SDK::TDelegate<void __cdecl(void)>*, int, bool, bool, SDK::FString*>(FRequestAsyncLoad, GetStreamableManager(), &ptr, path, &delasync, 0, true, false, &str);
		Sleep(num);
		CallGameFunction<__int64, void*, float, bool>(FWaitUntilComplete, ptr.ptr, 0.0, 1);
		//If this becomes problematic or in need of change maybe try to hook FEngineLoop::Tick and be able to send in lambdas. that should run code on the main thread.
	}

	inline void* GetPlatformFile()
	{
		return CallGameFunction<void*, void*>(FGetPlatformFile, CallGameFunction<void*>(FGetPlatformFileManager));
	}

	inline void SetPath(SDK::FakeSoftObjectPtr::FSoftObjectPath* This, SDK::FName pathname)
	{
		return CallGameFunction<void, SDK::FakeSoftObjectPtr::FSoftObjectPath*, SDK::FName>(FSetPath, This, pathname);
	}

	inline SDK::TArray<SDK::FString> GetVFSFiles()
	{
		SDK::FString gamepath = SDK::UBlueprintPathsLibrary::ProjectContentDir();
		SDK::TArray<SDK::FString> ret;
		CallGameFunction<void, void*, SDK::TArray<SDK::FString>*, const wchar_t*, const wchar_t*>(FFindFilesRecursively, GetPlatformFile(), &ret, gamepath.CStr(), L".uasset");
		return ret;
	}

	inline std::wstring _to_wstring(const std::string& str)
	{
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
		std::wstring wstr(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
		wstr.pop_back(); // remove null terminator
		return wstr;
	}

	//Example WBP_PropertyContainer_C
	inline void AttemptLoadClass(const char* classname)
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

		if (res == L"NONE") { std::cout << "Failed to load Class!" << std::endl; return; }

		SDK::TSoftClassPtr<SDK::UClass> ptr = SDK::TSoftClassPtr<SDK::UClass>();
		const SDK::FName path = SDK::UKismetStringLibrary::Conv_StringToName(SDK::FString(res.c_str()));//FString is volatile and wrong. only use as const for final step moving on.
		SetPath(&ptr.ObjectID, path);
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
			RequestAsyncLoad_D(&ptr.ObjectID, 50);
		}
	}
	#endif // _DEBUG

	#pragma endregion
}

template<typename T>
inline SDK::UClass* GetClassInternal(const char* clsobjname)
{
	SDK::UClass* objcls = T::StaticClass();
	if (objcls == nullptr) {
		_spawnutils::AttemptLoadClass(clsobjname);
		for (int i = 0; i < 5; i++) //The class should have loaded, but give it some time if not.
		{
			objcls = T::StaticClass();
			if (objcls) {
				return objcls;
			}
			Sleep(200);
		}
	}
	return nullptr;
}

//Example parameter UBP_DamageType_FuelExplosion_C
template<typename T>
inline T* SpawnObjectInternal(SDK::UObject* outerobj, const char* objclsname)
{
	SDK::UClass* objcls = T::StaticClass();
	if (objcls == nullptr) {
		_spawnutils::AttemptLoadClass(objclsname); //This should only be called on UBP classes.
		for (_itor(5)) //The class should have loaded, but give it some time if not.
		{
			objcls = T::StaticClass();
			if (objcls) {
				break;
			}
			Sleep(200);
		}
	}
	return static_cast<T*>(SDK::UGameplayStatics::SpawnObject(objcls, outerobj));
}

template<typename T>
inline T* CreateWidgetInternal(SDK::TSubclassOf<SDK::UUserWidget> UserWidgetClass, const char* WidgetClassName)
{

	if (UserWidgetClass == nullptr) {
		_spawnutils::AttemptLoadClass(WidgetClassName);
		for (_itor(5)) //The class should have loaded, but give it some time if not.
		{
			UserWidgetClass = T::StaticClass();
			if (UserWidgetClass) {
				break;
			}
			Sleep(200);
		}
	}

	return static_cast<T*>(CallGameFunction<SDK::UUserWidget*, SDK::UWorld*, SDK::TSubclassOf<SDK::UUserWidget>, SDK::FName>(FCreateWidget, SDK::UWorld::GetWorld(), UserWidgetClass, SDK::FName()));
}

template<typename T>
inline T* SpawnActorInternal(SDK::AActor* outeract, const char* objclsname)
{
	if (outeract == nullptr)
	{
		std::cout << "outer was null!" << std::endl;
		return nullptr;//requires it.
	}
	SDK::FTransform transform = outeract->GetTransform();
	SDK::UClass* objcls = T::StaticClass();
	if (objcls == nullptr) {
		_spawnutils::AttemptLoadClass(objclsname);
		for (_itor(5)) //The class should have loaded, but give it some time if not.
		{
			objcls = T::StaticClass();
			if (objcls) {
				break;
			}
			Sleep(200);
		}
	}
	else std::cout << "class was present!" << std::endl;
	SDK::ESpawnActorCollisionHandlingMethod method = SDK::ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SDK::AActor* act = SDK::UGameplayStatics::BeginDeferredActorSpawnFromClass(SDK::UWorld::GetWorld(), objcls, transform, method, outeract);
	return static_cast<T*>(SDK::UGameplayStatics::FinishSpawningActor(act, transform));
}

template<typename T>
inline T* SpawnActorInternal(SDK::FTransform transform, SDK::AActor* outeract, const char* objclsname, bool deferred)
{
	SDK::UClass* objcls = T::StaticClass();
	if (objcls == nullptr) {
		_spawnutils::AttemptLoadClass(objclsname);
		for (_itor(5)) //The class should have loaded, but give it some time if not.
		{
			objcls = T::StaticClass();
			if (objcls) {
				break;
			}
			Sleep(200);
		}
	}
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
	SDK::UClass* objcls = T::StaticClass();
	if (objcls == nullptr) {
		_spawnutils::AttemptLoadClass(objclsname);
		for (_itor(5)) //The class should have loaded, but give it some time if not.
		{
			objcls = T::StaticClass();
			if (objcls) {
				break;
			}
			Sleep(200);
		}
	}
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
	SDK::UClass* objcls = T::StaticClass();
	if (objcls == nullptr) {
		_spawnutils::AttemptLoadClass(objclsname);
		for (_itor(5)) //The class should have loaded, but give it some time if not.
		{
			objcls = T::StaticClass();
			if (objcls) {
				break;
			}
			Sleep(200);
		}
	}
	SDK::ESpawnActorCollisionHandlingMethod method = SDK::ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SDK::AActor* act = SDK::UGameplayStatics::BeginDeferredActorSpawnFromClass(SDK::UWorld::GetWorld(), objcls, transform, method, outeract);
	SDK::UGameplayStatics::FinishSpawningActor(act, transform);
	return static_cast<T*>(act);
}