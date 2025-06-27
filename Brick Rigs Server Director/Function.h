#pragma once
#include <libloaderapi.h>

template<typename TRet, typename... TArgs>
inline TRet CallGameFunction(unsigned long long addr, TArgs... args) 
{
	unsigned long long FunctionPTR = (unsigned long long)GetModuleHandle(NULL) + addr;
	using FunctionFn = TRet(__fastcall*)(TArgs...);
	FunctionFn OnFunction = reinterpret_cast<FunctionFn>(FunctionPTR);
	return OnFunction(std::forward<TArgs>(args)...);
}