/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     Hook.h			                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    25 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <exception>
#include <MinHook.h>
#include <windows.h>
#include <Psapi.h>

class HookingException : public std::exception
{
public:
	explicit HookingException(const char* message) : sErrorMessage(message) {}

	const char* what() const noexcept override {
		return sErrorMessage;
	}
private:
	const char* sErrorMessage;
};

template <typename Ret, typename... Args>  
class Hook  
{  
public:  
    Hook(const char* pat, const char* mak, Ret(__fastcall* hookFunc)(Args...)); //pattern and mask, use the \x00 format on sigs, x and ? on masks (? is wildcard)
    Hook(unsigned long long addr, Ret(__fastcall* hookFunc)(Args...));  //The address base is calculated when creating the object. Only the offset
	Hook(Ret(__fastcall* ptr)(Args...), Ret(__fastcall* hookFunc)(Args...)); //Use reinterpret_cast<Ret(__fastcall*)(Args...)>(vtable[index]) when inputing a vtable entry
    ~Hook();  

private:  
    bool enabled;  
    bool initalized;  
	unsigned long long FunctionPointer;
    const char* pattern;  
    const char* mask;  
    using Function_t = Ret(__fastcall*)(Args...);  
    bool Init();  

protected:  
    Function_t OriginalFunction;  
	Function_t hookedFunction;

public:
    void Enable();
    void Disable();

protected:
	static uintptr_t FindPattern(const char* pattern, const char* mask, uintptr_t base, size_t size);
	static uintptr_t GetModuleBase();
	static uintptr_t GetModuleSize();
};

template<typename Ret, typename ...Args>
Hook<Ret, Args...>::Hook(const char* pat, const char* mak, Ret(__fastcall* hookFunc)(Args...))
{
	pattern = pat;
	mask = mak;
	enabled = false;
	initalized = false;
	FunctionPointer = 0;
	OriginalFunction = nullptr;
	hookedFunction = hookFunc;

	if (!Init()) throw HookingException("Signature Not Found!");
}

template<typename Ret, typename ...Args>
Hook<Ret, Args...>::Hook(unsigned long long addr, Ret(__fastcall* hookFunc)(Args...))
{
	pattern = "None";
	mask = "None";
	enabled = false;
	initalized = false;
	FunctionPointer = (unsigned long long)(GetModuleHandle(NULL)) + addr;
	OriginalFunction = nullptr;
	hookedFunction = hookFunc;

	if (!Init()) throw HookingException("Initilization Error");
}

template<typename Ret, typename ...Args>
inline Hook<Ret, Args...>::Hook(Ret(__fastcall* ptr)(Args...), Ret(__fastcall* hookFunc)(Args...))
{
	pattern = "None";
	mask = "None";
	enabled = false;
	initalized = false;
	FunctionPointer = ptr;
	OriginalFunction = nullptr;
	hookedFunction = hookFunc;

	if (!Init()) throw HookingException("Initilization Error");
}

template<typename Ret, typename ...Args>
Hook<Ret, Args...>::~Hook()
{
	Disable();
	MH_RemoveHook((LPVOID)FunctionPointer);
	OriginalFunction = nullptr;
}

template<typename Ret, typename ...Args>
bool Hook<Ret, Args...>::Init() {
	if (initalized) return false;
	if (FunctionPointer == 0) FunctionPointer = FindPattern(pattern, mask, GetModuleBase(), GetModuleSize());
	if (FunctionPointer == 0) return false;
	MH_STATUS ret = MH_CreateHook((LPVOID)FunctionPointer, hookedFunction, (void**)&OriginalFunction);
	initalized = true;
	return ret == MH_OK;
}

template<typename Ret, typename ...Args>
void Hook<Ret, Args...>::Enable() {
	if (!initalized || enabled) return;
	MH_STATUS status = MH_EnableHook((LPVOID)FunctionPointer);
	if (status != MH_OK) throw HookingException(MH_StatusToString(status));
	enabled = true;
}

template<typename Ret, typename ...Args>
void Hook<Ret, Args...>::Disable() {
	if (!initalized || !enabled) return;
	MH_STATUS status = MH_DisableHook((LPVOID)FunctionPointer);
	if (status != MH_OK) throw HookingException(MH_StatusToString(status));
	enabled = false;
}

template<typename Ret, typename ...Args>
inline uintptr_t Hook<Ret, Args...>::FindPattern(const char* pattern, const char* mask, uintptr_t base, size_t size)
{
	size_t patternLen = strlen(mask);

	for (size_t i = 0; i < size - patternLen; i++) {
		bool found = true;

		for (size_t j = 0; j < patternLen; j++) {
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

template<typename Ret, typename ...Args>
inline uintptr_t Hook<Ret, Args...>::GetModuleBase()
{
	return (uintptr_t)GetModuleHandle(NULL);
}

template<typename Ret, typename ...Args>
inline uintptr_t Hook<Ret, Args...>::GetModuleSize()
{
	MODULEINFO info = {};
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &info, sizeof(info));
	return info.SizeOfImage;
}
