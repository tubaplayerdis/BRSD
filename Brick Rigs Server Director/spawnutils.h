#pragma once
#include "Function.h"
#include "offsets.h"
#include <SDK.hpp>

/// <summary>
/// Spawns a new UObject using internal UE systems. Use when creating UObjects 
/// </summary>
/// <param name="cls">The class of the new object. Not the UClass. Ex: SDK::UBrickBorder </param>
/// <param name="out">A pointer to the outer object the new object should be created with</param>
/// <returns>A pointer to the new object</returns>
#define Spawn(cls, out) SpawnObjectInternal<cls>(out, ## #cls)

/// <summary>
/// Creates a new UUserWidget derived object using internal UE systems.
/// Use Spawn() for UWidget derieved objects 
/// </summary>
/// <param name="cls">The class of the new widget. Not the UClass. Ex: SDK::UWPB_PropertyContainer_C </param>
/// <returns>A pointer to the new widget</returns>
#define Create(cls) CreateWidgetInternal<cls>(cls::StaticClass(), ## #cls)

/// <summary>
/// Adds a UClass refrence internally for use again later
/// </summary>
/// <param name="cls">The class of the widget. Not the UClass. Ex: SDK::UWPB_PropertyContainer_C </param>
/// <returns>None</returns>
#define Load(cls)


#define _itor(num) int i = 0; i < num; i++

namespace _spawnutils
{
	inline auto cb = new std::function<void()>([]() {
		std::cout << "Hello from callback!" << std::endl;
	});


	#pragma region helpers
	typedef SDK::TDelegate<void __cdecl(SDK::FName const&, SDK::UPackage*, __int32)> LPADelegate;

	inline bool DoesPackageExist(const SDK::FString& LongPackageName, const SDK::FGuid* Guid, SDK::FString* OutFilename, bool InAllowTextFormats = true)
	{
		return CallGameFunction<bool, const SDK::FString&, const SDK::FGuid*, SDK::FString*, bool>(FDoesPackageExist, LongPackageName, Guid, OutFilename, InAllowTextFormats);
	}

	inline SDK::int32 LoadPackageAsync(const SDK::FString& InName, LPADelegate InCompletionDelegate, int InPackagePriority = 0, __int32 InPackageFlags = 0, SDK::int32 InPIEInstanceID = -1)
	{
		return CallGameFunction<SDK::int32, const SDK::FString&, LPADelegate, int, __int32, SDK::int32>(FLoadPackageAsync, InName, InCompletionDelegate, InPackagePriority, InPackageFlags, InPIEInstanceID);
	}

	inline void* GetStreamableManager()
	{
		return CallGameFunction<void*>(FGetStreamableManager);
	}


	struct falseSharedPtr
	{
		void* ptr;
		uint8_t pad[0x8];
	};

	inline void RequestSyncLoad(SDK::FakeSoftObjectPtr::FSoftObjectPath path)
	{
		falseSharedPtr ptr{};
		ptr.ptr = nullptr;
		SDK::TAllocatedArray<SDK::FakeSoftObjectPtr::FSoftObjectPath> allo = SDK::TAllocatedArray<SDK::FakeSoftObjectPtr::FSoftObjectPath>(1);
		allo.Add(path);
		UC::FString str = UC::FString(L"LoadAssetList");
		if (GetStreamableManager()) std::cout << "valid!" << std::endl;
		CallGameFunction<falseSharedPtr*, void*, void*, SDK::TArray<SDK::FakeSoftObjectPtr::FSoftObjectPath>*, bool, SDK::FString*>(FRequestSyncLoad, GetStreamableManager(), &ptr, &allo, true, &str);//FStreamableManager::RequestSyncLoad
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

	inline SDK::TDelegate<void __cdecl(SDK::FName const&, SDK::UPackage*, __int32)> del = SDK::TDelegate<void __cdecl(SDK::FName const&, SDK::UPackage*, __int32)>();

	//Example WBP_PropertyContainer_C
	inline void AttemptLoadClass(const char* classname)
	{
		std::wstring lookfor = _to_wstring(classname);
		lookfor.pop_back();
		lookfor.pop_back();
		lookfor.append(L".");//Remove the _C and add a dot so that we know when the name ends.
		std::cout << "attempting find: "; 
		std::wcout << lookfor << std::endl;
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

				result = L"/Game/" + result;

				res = result;
				std::wcout << L"Result: " << res << std::endl;
				break;
			}
		}

		if (res == L"NONE") { std::cout << "Failed to load Class!" << std::endl; return; }

		const UC::FString path = UC::FString(res.c_str());//FString is volatile and wrong. only use as const for final step moving on.
		LoadPackageAsync(path, del);
		return;
	}
	#pragma endregion
}

//Example parameter UBP_DamageType_FuelExplosion_C
template<typename T>
inline T* SpawnObjectInternal(SDK::UObject* outerobj, const char* objclsname)
{
	SDK::UClass* objcls = T::StaticClass();
	if (objcls == nullptr) {
		std::string wcn = std::string(objclsname);
		if (wcn.starts_with("UBP")) {
			_spawnutils::AttemptLoadClass(wcn.substr(1).c_str()); //This should only be called on UBP classes.
			for (_itor(10)) //Give 1 second max to load the package.
			{
				Sleep(1);
				objcls = T::StaticClass();
				if (objcls) {
					std::cout << "obj found!" << std::endl; break;
				}
			}
		}
	}
	return static_cast<T*>(SDK::UGameplayStatics::SpawnObject(objcls, outerobj));
}

template<typename T>
inline T* CreateWidgetInternal(SDK::TSubclassOf<SDK::UUserWidget> UserWidgetClass, const char* WidgetClassName)
{

	if (UserWidgetClass == nullptr) {
		std::string wcn = std::string(WidgetClassName);
		_spawnutils::AttemptLoadClass(wcn.substr(wcn.find_first_of('U') + 1).c_str());
		for (_itor(10)) //Give 1 second max to load the package.
		{
			Sleep(1);
			UserWidgetClass = T::StaticClass();
			if (UserWidgetClass) {
				std::cout << "obj found!" << std::endl; break;
			}
		}
	}

	return static_cast<T*>(CallGameFunction<SDK::UUserWidget*, SDK::UWorld*, SDK::TSubclassOf<SDK::UUserWidget>, SDK::FName>(FCreateWidget, SDK::UWorld::GetWorld(), UserWidgetClass, SDK::FName()));
}