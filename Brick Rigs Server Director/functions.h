/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     functions.h			                                      */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <SDK.hpp>
#include "Function.h"
#include "offsets.h"

namespace FLinearColor
{
	/*
	* Colors are 0-255 and A is 0-1
	* FLinearColor expect values from 0-1 not 0-255, given that, 0-255 values are taken and mapped down in the function.
	*/
	inline SDK::FLinearColor CreateLinearColor(float R, float G, float B, float A)
	{
		SDK::FLinearColor ret = SDK::FLinearColor();
		ret.A = A;
		ret.R = R / 255.0f;
		ret.G = G / 255.0f;
		ret.B = B / 255.0f;
		return ret;
	}


	/*
	* Colors are 0-1 and A is 0-1
	* FLinearColor expect values from 0-1 not 0-255, and since this is the normal function, inputs are 0-1.
	*/
	inline SDK::FLinearColor CreateLinearColorNorm(float R, float G, float B, float A)
	{
		SDK::FLinearColor ret = SDK::FLinearColor();
		ret.A = A;
		ret.R = R;
		ret.G = G;
		ret.B = B;
		return ret;
	}
}

namespace FSlateColor
{
	/*Use this by passing in a valid linear color and a FSlateColor created by the default constructor*/
	inline void FSlateColorConstructor(SDK::FSlateColor* This, SDK::FLinearColor* InColor)
	{
		return CallGameFunction<void, SDK::FSlateColor*, SDK::FLinearColor*>(FindPatternF("\x0F\x10\x02\x33\xC0\x0F\x11\x01\x48\x89\x41\x18\x48\x89\x41\x20\x48\x8B\xC1\xC6\x41\x10\x00\xC3", "xxxxxxxxxxxxxxxxxxxxxxxx"), This, InColor);
	}

	/*
	* Colors are 0-255 and A is 0-1
	*/
	inline SDK::FSlateColor CreateSlateColor(float R, float G, float B, float A)
	{
		SDK::FSlateColor ret;
		SDK::FLinearColor col = FLinearColor::CreateLinearColor(R, G, B, A);
		FSlateColorConstructor(&ret, &col);
		return ret;
	}
}

namespace FBrickChatMessage
{
	inline void FBrickChatMessageConstructor(SDK::FBrickChatMessage* This, SDK::EChatMessageType ChatType, SDK::ABrickPlayerController* PC)
	{
		return CallGameFunction<void, SDK::FBrickChatMessage*, SDK::EChatMessageType, SDK::ABrickPlayerController*>(FFBrickChatMessageConstructor, This, ChatType, PC);
	}
}

inline void SynchronizeProperties(SDK::UBrickBorder* This)
{
	return CallGameFunction<void, SDK::UBrickBorder*>(FSynchronizeProperties_UBB, This);
}

inline void SynchronizeProperties(SDK::UBrickScrollBox* This)
{
	return CallGameFunction<void, SDK::UBrickScrollBox*>(FSynchronizeProperties_UBSB, This);
}

inline char Initalize(SDK::UUserWidget* widget)
{
	if (!widget) return false;
	using InitalizeFn = bool(__fastcall*)(SDK::UUserWidget*);
	void** vtable = *(void***)widget;
	InitalizeFn InitalizeFunc = reinterpret_cast<InitalizeFn>(vtable[VInitalize]);
	return InitalizeFunc(widget);
}



namespace UMainWidgetBase
{
	inline void UpdateInputMode(SDK::UMainWidgetBase* Base)
	{
		return CallGameFunction<void, SDK::UMainWidgetBase*>(FUpdateInputMode, Base);
	}
}

namespace UModHookSubsystem
{
	inline SDK::UModHookSubsystem* GetModHookSubsystem() 
	{
		return CallGameFunction<SDK::UModHookSubsystem*>(FGetModHookSubsystem);
	}
}

namespace UNetDriver
{
	inline bool isServer(SDK::UNetDriver* driver)
	{
		if (!driver) return false;
		using IsServerFn = bool(__fastcall*)(SDK::UNetDriver*);
		void** vtable = *(void***)driver;
		IsServerFn IsServerFunc = reinterpret_cast<IsServerFn>(VisServer);
		return IsServerFunc(driver);
	}
}

namespace ConstructorHelpersInternal
{
	inline SDK::UClass* FindOrLoadClass(SDK::FString& path, SDK::UClass* baseClass)
	{
		return CallGameFunction<SDK::UClass*, SDK::FString&, SDK::UClass*>(FFindOrLoadClass, path, baseClass);
	}
}

inline SDK::UPackage* LoadPackage(SDK::UPackage* InOuter, const wchar_t* InLongPackageName, unsigned int LoadFlags = 0, void* InReaderOverride = nullptr, void* InstancingContext = nullptr)
{
	return CallGameFunction<SDK::UPackage*, SDK::UPackage*, const wchar_t*, unsigned int, void*, void*>(FLoadPackage, InOuter, InLongPackageName, LoadFlags, InReaderOverride, InstancingContext);
}

typedef SDK::TDelegate<void __cdecl(SDK::FName const&, SDK::UPackage*, __int32)> LPADelegate;

inline SDK::int32 LoadPackageAsync(const SDK::FString& InName, LPADelegate InCompletionDelegate, int InPackagePriority = 0, __int32 InPackageFlags = 0, SDK::int32 InPIEInstanceID = -1)
{
	return CallGameFunction<SDK::int32, const SDK::FString&, LPADelegate, int, __int32, SDK::int32>(FLoadPackageAsync, InName, InCompletionDelegate, InPackagePriority, InPackageFlags, InPIEInstanceID);
}

namespace FPackageName
{
	inline bool DoesPackageExist(const SDK::FString& LongPackageName, const SDK::FGuid* Guid, SDK::FString* OutFilename, bool InAllowTextFormats = true)
	{
		return CallGameFunction<bool, const SDK::FString&, const SDK::FGuid*, SDK::FString*, bool>(FDoesPackageExist, LongPackageName, Guid, OutFilename, InAllowTextFormats);
	}
}

namespace UBrickAssetManager
{
	inline void LoadAssetsFromPathInternal(SDK::TArray<SDK::UObject*>* OutAssets, const SDK::FString path, SDK::UClass* BaseClass, bool bIsBlueprintClass)
	{
		SDK::UBrickAssetManager* manager = CallGameFunction<SDK::UBrickAssetManager*>(FGet_UBAM);
		if (!manager) std::cout << "incoming explosion!" << std::endl;
		return CallGameFunction<void, SDK::UBrickAssetManager*, SDK::TArray<SDK::UObject*>*, const SDK::FString, bool>(FLoadAssetsFromPathInternal, manager, OutAssets, path, BaseClass, bIsBlueprintClass);
	}
}

namespace FSoftObjectPath
{
	inline void SetPath(SDK::FakeSoftObjectPtr::FSoftObjectPath* This, SDK::FName pathname)
	{
		return CallGameFunction<void, SDK::FakeSoftObjectPtr::FSoftObjectPath*, SDK::FName>(FSetPath, This, pathname);
	}
}

inline SDK::UObject* LoadSynchronous(SDK::FSoftObjectPtr* This)
{
	return CallGameFunction<SDK::UObject*, SDK::FSoftObjectPtr*>(FLoadSynchronous, This);
}

inline void FlushAsyncLoading(int packageID)
{
	return CallGameFunction<void, int>(FFlushAsyncLoading, packageID);
}

inline void* GetPlatformFile()
{
	return CallGameFunction<void*, void*>((unsigned long long)GetModuleHandleW(0) + 0x0F1EDD0, CallGameFunction<void*>((unsigned long long)GetModuleHandleW(0) + 0x0F19360));
}

inline SDK::TArray<SDK::FString> GetVFSFiles()
{
	SDK::FString gamepath = SDK::UBlueprintPathsLibrary::ProjectContentDir();
	SDK::TArray<SDK::FString> ret;
	CallGameFunction<void, void*, SDK::TArray<SDK::FString>*, const wchar_t*, const wchar_t*>((unsigned long long)GetModuleHandleW(0) + 0x0EEF8C0, GetPlatformFile(), &ret, gamepath.CStr(), L".uasset");
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

inline SDK::TDelegate<void __cdecl(SDK::FName const&, SDK::UPackage*, __int32)> del = SDK::TDelegate<void __cdecl(SDK::FName const&, SDK::UPackage*, __int32)>();

inline void AttemptLoadClass(const char* classname)
{
	std::cout << "raw: " << classname << std::endl;
	std::wstring lookfor = _to_wstring(classname);
	lookfor.pop_back();
	lookfor.pop_back();
	lookfor.append(L".");
	std::wcout << "attempting find: " << lookfor << std::endl;
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
				break; // or handle error
			}

			// Find the next "BrickRigs/" after the first one (skipping "BrickRigs/Content/")
			size_t secondBrickRigs = original.find(target, firstBrickRigs + target.length());
			if (secondBrickRigs == std::wstring::npos) {
				break; // or handle error
			}

			// Extract from the second "BrickRigs/"
			std::wstring result = original.substr(secondBrickRigs);

			// Remove the ".uasset" extension
			size_t dotPos = result.rfind('.');
			if (dotPos != std::wstring::npos) {
				result = result.substr(0, dotPos);
			}

			result = L"/Game/" + result;

			res = result;
			break;
		}
	}

	if (res == L"NONE") { std::cout << "Failed to load Class!" << std::endl; return; }

	const UC::FString path = UC::FString(res.c_str());
	LoadPackageAsync(path, del);
	Sleep(100);
	return;
}
