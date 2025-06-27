#pragma once
#include <windows.h>
#include <libloaderapi.h>
#include <processthreadsapi.h>
#include <Psapi.h>

template<typename TRet, typename... TArgs>
inline TRet CallGameFunction(unsigned long long addr, TArgs... args) 
{
	unsigned long long FunctionPTR = (unsigned long long)GetModuleHandle(NULL) + addr;
	using FunctionFn = TRet(__fastcall*)(TArgs...);
	FunctionFn OnFunction = reinterpret_cast<FunctionFn>(FunctionPTR);
	return OnFunction(std::forward<TArgs>(args)...);
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