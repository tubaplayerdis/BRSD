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
#include <BR-SDK.hpp>
#include "offsets.h"
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>
#include <iomanip>
#include <iostream>

namespace testing
{
	inline void print_uint8_hex(uint8_t value) {
		std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
			<< static_cast<int>(value);
	}

	inline unsigned long long FindPatternF(const char* pattern, const char* mask, unsigned long long base, unsigned __int64 size)
	{
		const unsigned __int64 patternLen = strlen(mask);
		if (patternLen == 0) {
			return 0;
		}

		// 1. Create the bad-character skip table
		std::vector<unsigned __int64> skipTable(256, patternLen);
		for (unsigned __int64 i = 0; i < patternLen - 1; ++i) {
			if (mask[i] != '?') {
				skipTable[static_cast<unsigned char>(pattern[i])] = patternLen - 1 - i;
			}
		}

		const unsigned long long searchEnd = base + size - patternLen;
		unsigned long long currentPos = base;

		while (currentPos <= searchEnd) {
			// 2. Compare from the end of the pattern backwards
			bool match = true;
			for (int j = patternLen - 1; j >= 0; --j) {
				if (mask[j] != '?' && pattern[j] != *(char*)(currentPos + j)) {
					// 3. On mismatch, use the skip table to jump forward
					// The character from the memory text determines the jump distance.
					const unsigned char mismatched_char = *(unsigned char*)(currentPos + patternLen - 1);
					currentPos += skipTable[mismatched_char];
					match = false;
					break;
				}
			}

			if (match) {
				return currentPos; // Found it
			}
		}

		return 0; // Not found
	}

	inline unsigned long long FindPatternS(const char* pattern, const char* mask, unsigned long long base, unsigned __int64 size)
	{
		unsigned __int64 patternLen = strlen(mask);

		for (unsigned __int64 i = 0; i < size - patternLen; i++) {
			bool found = true;

			for (unsigned __int64 j = 0; j < patternLen; j++) {
				if (mask[j] != '?' && pattern[j] != *(char*)(base + i + j)) {
					found = false;
					break;
				}
			}

			if (found)
				return base + i;
		}

		return 0;
	}

	inline bool GetTextSection(unsigned long long& textBase, unsigned __int64& textSize)
	{
		uintptr_t moduleBase = (unsigned long long)GetModuleHandle(NULL);
		auto dos = (PIMAGE_DOS_HEADER)moduleBase;
		auto nt = (PIMAGE_NT_HEADERS)(moduleBase + dos->e_lfanew);

		auto section = IMAGE_FIRST_SECTION(nt);
		for (unsigned i = 0; i < nt->FileHeader.NumberOfSections; ++i, ++section)
		{
			if (strncmp((char*)section->Name, ".text", 5) == 0)
			{
				textBase = moduleBase + section->VirtualAddress;
				textSize = section->Misc.VirtualSize;
				return true;
			}
		}

		return false;
	}

	inline unsigned long long FindPatternAll(const char* pattern, const char* mask)
	{
		HMODULE hMods[1024];
		DWORD cbNeeded;
		HANDLE hProcess = GetCurrentProcess();
		if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
			for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
				TCHAR szModName[MAX_PATH];

				GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR));

				std::cout << "Scanning: ";
				std::wcout << szModName << L"\n";

				uintptr_t base = reinterpret_cast<uintptr_t>(hMods[i]);

				MODULEINFO modInfo;
				GetModuleInformation(hProcess, hMods[i], &modInfo, sizeof(modInfo));
				unsigned __int64 size = modInfo.SizeOfImage;


				unsigned __int64 patternLen = strlen(mask);

				for (unsigned __int64 i = 0; i < size - patternLen; i++) {
					bool found = true;

					for (unsigned __int64 j = 0; j < patternLen; j++) {
						if (mask[j] != '?' && pattern[j] != *(char*)(base + i + j)) {
							found = false;
							break;
						}
					}

					if (found)
						return base + i;
				}

			}

			return 0;
		}
	}

	inline uint8_t hex_char_to_value(char c) {
		if (c >= '0' && c <= '9') return c - '0';
		if (c >= 'A' && c <= 'F') return c - 'A' + 10;
		if (c >= 'a' && c <= 'f') return c - 'a' + 10;
		return 0; // Invalid character
	}

	inline uint8_t hex_string_to_uint8(const std::string& hex_str) {
		if (hex_str.length() != 2) return 0;
		return (hex_char_to_value(hex_str[0]) << 4) | hex_char_to_value(hex_str[1]);
	}

	inline void SignatureToPatternAndMask(const char* signature, std::vector<char>& pattern, std::string& mask) {
		pattern.clear();
		mask.clear();

		std::string sig_str = signature;
		std::stringstream ss(sig_str);
		std::string token;

		while (ss >> token) {
			if (token == "??") {
				// For wildcards, use any value (0x00) in pattern, '?' in mask
				pattern.push_back(0x00);
				mask += '?';
			}
			else {
				// Convert hex string to byte value
				if (token.length() == 2) {
					char value = static_cast<char>(std::stoi(token, nullptr, 16));
					pattern.push_back(value);
					mask += 'x';
				}
			}
		}

		// Ensure null termination for C-style strings
		pattern.push_back(0x00);
	}

	inline std::pair<const char*, const char*> ConvertSignature(const char* signature) {
		static std::vector<char> pattern;
		static std::string mask;

		SignatureToPatternAndMask(signature, pattern, mask);

		return { pattern.data(), mask.c_str() };
	}



	/// Resolve a signature to an address. Uses the format: "48 89 7C 24 ?? 41 56 48 83 EC ?? 48 8B FA 4C 8B F1 E8 ?? ?? ?? ??"
	/// @param signature signature to resolve
	/// @return address of the function representing the signature. 0 if not found.
	inline unsigned long long ResolveSignature(const char* signature)
	{
		//Only Scan Current Module
		unsigned long long base = 0;
		unsigned __int64 size = 0;
		if (!GetTextSection(base, size)) return 1;

		// Convert signature to pattern/mask format
		auto [pattern, mask] = ConvertSignature(signature);

		std::cout << "Pattern: ";
		for (size_t i = 0; i < strlen(mask); i++) {
			print_uint8_hex(static_cast<uint8_t>(pattern[i]));
			std::cout << " ";
		}
		std::cout << std::endl;
		std::cout << "Mask: " << mask << std::endl;
		std::cout << "Mask length: " << strlen(mask) << std::endl;

		// Use your working functions
		unsigned long long addr = FindPatternAll(pattern, mask);
		if (addr == 0) {
			addr = FindPatternS(pattern, mask, base, size);
		}

		return addr;
	}
}


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

inline char SetRootComponent(SDK::AActor* This, SDK::USceneComponent* component)
{
	return CallGameFunction<char, SDK::AActor*, SDK::USceneComponent*>(BASE + 0x21D9430, This, component);
}

inline SDK::FUGCFileInfo* GetSelectedFileInfo(SDK::UUGCBrowserWidget* This)
{
	if (!This) return nullptr;
	return CallGameFunction<SDK::FUGCFileInfo*, SDK::UUGCBrowserWidget*>(FGetSelectedFileInfo, This);
}
