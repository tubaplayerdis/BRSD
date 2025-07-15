/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     Function.h					                              */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    6 July 2025                                                 */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <windows.h>
#include <libloaderapi.h>
#include <processthreadsapi.h>
#include <Psapi.h>


template<typename TRet, typename... TArgs>
inline TRet CallGameFunction(unsigned long long addr, TArgs... args) 
{
	using FunctionFn = TRet(__fastcall*)(TArgs...);
	FunctionFn OnFunction = reinterpret_cast<FunctionFn>(addr);
	return OnFunction(std::forward<TArgs>(args)...);
}

template<typename TRet, typename... TArgs>
inline TRet CallVTableFunction(int index ,void* Object, TArgs... args)
{
    using FunctionFn = TRet(__fastcall*)(void*, TArgs...);
    void** vtable = *(void***)Object;
    FunctionFn FunctionFunc = reinterpret_cast<FunctionFn>(index/sizeof(void*));
    return FunctionFunc(Object, std::forward<TArgs>(args)...);
}

inline unsigned long long FindPatternF(const char* pattern, const char* mask)
{
    unsigned long long base = (unsigned long long)GetModuleHandle(NULL);
    MODULEINFO info = {};
    GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &info, sizeof(info));
    unsigned __int64 size = (unsigned __int64)info.SizeOfImage;
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