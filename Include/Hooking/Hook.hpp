/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     Hook.h			                                          */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    25 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.5                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include "MinHook/MinHook.h"
#include "Signature.hpp"
#include <libloaderapi.h>
#include <cassert>
#include <string>
#include <utility>
#include <stdexcept>

/// Creates a Hook object using smart pointers and lambdas. Creates a function called "name()" (Where name is the first parameter of the macro) that can be used to access the Hook object created by this macro.
/// @note Creates a function called "name()" (Where name is the first parameter of the macro) that can be used to access the Hook object created by this macro.
/// @note Does not need to be explicitly deleted but is recommended to be explicitly disabled.
/// @param name Name of the Hook, used when defining the access function .
/// @param addr Address of the function to hook.
/// @param lamb Lambda that represents the trampoline function, IE, the replacement function.
/// @param sig Pseudo-signature of the Hook, in format: Ret(Args).
#define HOOK(name, addr, lamb, sig) \
std::unique_ptr<Hook<sig>>& name##(); \
inline std::unique_ptr<Hook<sig>> H_##name = std::make_unique<Hook<sig>>(addr, lamb); \
inline std::unique_ptr<Hook<sig>>& name##() { return H_##name; } \

/// Creates a Hook object using smart pointers and lambdas. Also created a function called "name()" (Where name is the first parameter of the macro) to be used to access the Hook object created by this macro. Does not need to be explicitly deleted but is recommended to be explicitly disabled.
/// @param name Name of the Hook, used when defining the access function .
/// @param pat Pattern of the signature to scan for, in format: \x00.
/// @param mask Mask of the signature to scan for, using x for matches and ? for wildcards.
/// @param lamb Lambda that represents the trampoline function, IE, the replacement function.
/// @param sig Pseudo-signature of the Hook, in format: Ret(Args).
/// @param ... Variadic argument that can be used to define the SearchType of the pattern scanning.
#define HOOK_PATTERN(name, pat, mask, lamb, sig, ...) \
std::unique_ptr<Hook<sig>>& name##(); \
inline std::unique_ptr<Hook<sig>> H_##name = std::make_unique<Hook<sig>>(pat, mask, lamb, __VA_ARGS__); \
inline std::unique_ptr<Hook<sig>>& name##() { return H_##name; } \

/// Registers the Hook with MinHook.
/// @note Equivalent to Create().
/// @param ptr Pointer to the hook object. use "name()"  (Where name is the first parameter of the macro that you defined earlier)
#define HOOK_INIT(ptr) ptr->Create()

/// Destroys the hook object explicitly. If the hook is running at the time this can cause issues.
/// @note If the hook is running at the time this is called, a crash will happen. If you used the HOOK() macro to define your Hook, you do not need to explicitly destroy it.
/// @param ptr Pointer to the hook object. use "name()"  (Where name is the first parameter of the macro that you defined earlier)
#define HOOK_DESTROY(ptr) ptr.reset()

/// Enables the Hook.
/// @note Equivalent to Enable()
/// @param ptr Pointer to the hook object. use "name()"  (Where name is the first parameter of the macro that you defined earlier)
#define HOOK_ENABLE(ptr) ptr->Enable()

/// Disables the Hook.
/// @note Equivalent to Disable()
/// @param ptr Pointer to the hook object. use "name()"  (Where name is the first parameter of the macro that you defined earlier)
#define HOOK_DISABLE(ptr) ptr->Disable()

/// Whether the Hook has been Initialized with MinHook.
/// @note Equivalent to IsInitialized()
/// @param ptr Pointer to the hook object. use "name()"  (Where name is the first parameter of the macro that you defined earlier)
/// @return The Hooks initialization state in regard to MinHook.
#define HOOK_IS_INIT(ptr) ptr->IsInitialized()

/// Calls the original Hooks function, bypassing the trampoline
/// @note Equivalent to CallOriginalFunction()
/// @param ptr Pointer to the hook object. use "name()"  (Where name is the first parameter of the macro that you defined earlier)
/// @param ... Variadic arguments of the original function to pass.
#define HOOK_CALL_ORIGINAL(ptr, ...) ptr->CallOriginalFunction(__VA_ARGS__)

/// Forward declaration allowing a pseudo-signature to use as template arguments.
template <typename>
class Hook;

/// Class representing a Hook. Smart abstraction layer over MinHook.
/// @tparam Ret Return type of the function to Hook.
/// @tparam Args Arguments of the function to Hook.
template <typename Ret, typename... Args>
class Hook<Ret(Args...)> final
{
public:

	/// Creates a Hook object using the specified BR-SDK compatible signature.
	/// @param signature BR-SDK compatible signature. See UsingBRSDK.MD Signature Formatting
	/// @param hookFunc Valid trampoline function pointer matching the signature specified in the Hook objects template signature.
	Hook(const char* signature, Ret (*hookFunc)(Args...));

	/// Creates a Hook object using the specified BR-SDK compatible signature.
	/// @param signature BR-SDK Signature. See UsingBRSDK.MD Signature Formatting
	/// @param hookFunc Valid trampoline function pointer matching the signature specified in the Hook objects template signature.
	Hook(const Signature& signature, Ret (*hookFunc)(Args...));

	/// Creates a Hook object using the specified address and trampoline function. Does not register the Hook with MinHook. Use Create() to register with MinHook.
	/// @param address Address of the function to hook.
	/// @param hookFunc Valid trampoline function pointer matching the signature specified in the Hook objects template signature.
	Hook(unsigned long long address, Ret(__fastcall* hookFunc)(Args...));

	/// Creates a Hook object using the specified function pointer and trampoline function. Does not register the Hook with MinHook. For VTable entries use reinterpret_cast<Ret(__fastcall*)(Args...)>(vtable[index]).
	/// @param pointer Function pointer of the specified signature in the Hook objects template signature.
	/// @param hookFunc Valid trampoline function pointer matching the signature specified in the Hook objects template signature.
	Hook(Ret(__fastcall* pointer)(Args...), Ret(__fastcall* hookFunc)(Args...));

	/// Explicit deconstructor for Hook. Disables then removes the Hook with MinHook. It is suggested to call Disable() or HOOK_DISABLE() instead of calling this if using the HOOK() macro to define the Hook.
	~Hook();

private:
	bool Enabled;
	bool Initialized;

	Signature FunctionSignature;
	using FunctionT = Ret(__fastcall*)(Args...);

	/// Initializes the Hook with MinHook and Searches patterns if necessary.
	/// @return Whether initialization completed successfully. Called in Create().
	bool Init();

public:
	FunctionT OriginalFunction;

protected:
	FunctionT HookedFunction;

public:

	/// Registers the Hook with MinHook but does not enable it
	void Create();

	/// Enables the Hook.
	void Enable();

	/// Disables the Hook.
	void Disable();

	/// Destroys the Hook.
	void Destroy();

	/// Calls the original function of the Hook, bypassing the trampoline
	/// @param args Args of the function as defined in the Hook objects template signature
	/// @return The return type defined in the Hook objects template signature
	Ret CallOriginalFunction(Args... args);

	/// Whether the Hook has been registered with MinHook
	/// @return Initialization state of the Hook in respect to its MinHook registration.
	bool IsInitialized() const;

	/// Whether the Hook is enabled
	/// @return Enabled state of the Hook.
	bool IsEnabled() const;
};

template <typename Ret, typename ... Args>
Hook<Ret(Args...)>::Hook(const char* signature, Ret(* hookFunc)(Args...)) : FunctionSignature(signature)
{
	Enabled = false;
	Initialized = false;
	OriginalFunction = nullptr;
	HookedFunction = hookFunc;
}

template<typename Ret, typename ...Args>
Hook<Ret(Args...)>::Hook(const Signature& signature, Ret(__fastcall* hookFunc)(Args...)) : FunctionSignature(signature)
{
	Enabled = false;
	Initialized = false;
	OriginalFunction = nullptr;
	HookedFunction = hookFunc;
}

template<typename Ret, typename ...Args>
Hook<Ret(Args...)>::Hook(unsigned long long addr, Ret(__fastcall* hookFunc)(Args...)) : FunctionSignature(addr)
{
	Enabled = false;
	Initialized = false;
	OriginalFunction = nullptr;
	HookedFunction = hookFunc;
}

template<typename Ret, typename ...Args>
Hook<Ret(Args...)>::Hook(Ret(__fastcall* pointer)(Args...), Ret(__fastcall* hookFunc)(Args...)) : FunctionSignature(reinterpret_cast<unsigned long long>(pointer))
{
	Enabled = false;
	Initialized = false;
	OriginalFunction = nullptr;
	HookedFunction = hookFunc;
}

template<typename Ret, typename ...Args>
Hook<Ret(Args...)>::~Hook()
{
	Disable();
	Destroy();
	OriginalFunction = nullptr;
}

template<typename Ret, typename ...Args>
bool Hook<Ret(Args...)>::Init() {
	if (Initialized) return false;
	if (FunctionSignature.GetPtr() == 0) return false;
	MH_STATUS ret = MH_CreateHook((LPVOID)FunctionSignature.GetPtr(), HookedFunction, (void**)&OriginalFunction);
	Initialized = ret == MH_OK;
	return ret == MH_OK;
}

template<typename Ret, typename ...Args>
void Hook<Ret(Args...)>::Create()
{
	if (Initialized) return;
	assert(Init());
}

template<typename Ret, typename ...Args>
void Hook<Ret(Args...)>::Enable()
{
	if (!Initialized) Create();
	if (!Initialized || Enabled) return;
	MH_QueueEnableHook((LPVOID)FunctionSignature.GetPtr());
	MH_ApplyQueued();
	Enabled = true;
}

template<typename Ret, typename ...Args>
void Hook<Ret(Args...)>::Disable()
{
	if (!Initialized || !Enabled) return;
	MH_QueueDisableHook((LPVOID)FunctionSignature.GetPtr());
	MH_ApplyQueued();
	Enabled = false;
}

template<typename Ret, typename ...Args>
void Hook<Ret(Args...)>::Destroy()
{
	if (!Initialized) return;
	if (!Enabled) Disable();
	MH_RemoveHook((LPVOID)FunctionSignature.GetPtr());
	Initialized = false;
	Enabled = false;
}

template<typename Ret, typename ...Args>
Ret Hook<Ret(Args...)>::CallOriginalFunction(Args ...args)
{
	assert(OriginalFunction != nullptr);
	return OriginalFunction(std::forward<Args>(args)...);
}

template<typename Ret, typename ...Args>
bool Hook<Ret(Args...)>::IsInitialized() const
{
	return Initialized;
}

template<typename Ret, typename ...Args>
bool Hook<Ret(Args...)>::IsEnabled() const
{
	return Enabled;
}