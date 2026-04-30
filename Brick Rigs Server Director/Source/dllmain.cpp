/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     dllmain.cpp                                                 */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    24 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <windows.h>
#include <__msvc_ostream.hpp>

#include "stdio.h"
#include "../Include/Main.h"
#include <Hooking/MinHook/MinHook.h>

// These are not called right now as they are not needed.
namespace {
    static void load_dependencies(HMODULE self)
    {
        wchar_t buffer[MAX_PATH];
        GetModuleFileNameW(self, buffer, MAX_PATH);
        std::wstring path(buffer);
        path = path.substr(0, path.find_last_of('\\')).append(L"\\");
        //Path is now where the DLL is located

        /*
        if (GetModuleHandle(L"dpp.dll") == NULL)                LoadLibrary(std::wstring(path + L"dpp.dll").c_str());
        if (GetModuleHandle(L"libcrypto-1_1-x64.dll") == NULL)  LoadLibrary(std::wstring(path + L"libcrypto-1_1-x64.dll").c_str());
        if (GetModuleHandle(L"libssl-1_1-x64.dll") == NULL)     LoadLibrary(std::wstring(path + L"libssl-1_1-x64.dll").c_str());
        if (GetModuleHandle(L"opus.dll") == NULL)               LoadLibrary(std::wstring(path + L"opus.dll").c_str());
        if (GetModuleHandle(L"zlib1.dll") == NULL)              LoadLibrary(std::wstring(path + L"zlib1.dll").c_str());
        */
    }

    static void release_dependencies(HMODULE self)
    {
        /*
        if (GetModuleHandle(L"dpp.dll") != NULL)                FreeLibrary(GetModuleHandle(L"dpp.dll"));
        if (GetModuleHandle(L"libcrypto-1_1-x64.dll") != NULL)  FreeLibrary(GetModuleHandle(L"libcrypto-1_1-x64.dll"));
        if (GetModuleHandle(L"libssl-1_1-x64.dll") != NULL)     FreeLibrary(GetModuleHandle(L"libssl-1_1-x64.dll"));
        if (GetModuleHandle(L"opus.dll") != NULL)               FreeLibrary(GetModuleHandle(L"opus.dll"));
        if (GetModuleHandle(L"zlib1.dll") != NULL)              FreeLibrary(GetModuleHandle(L"zlib1.dll"));
        */
    }
}

FILE* pStdIn = nullptr;
FILE* pStdOut = nullptr;
FILE* pStdErr = nullptr;

DWORD WINAPI MainThread(LPVOID lpReserved)
{
    HMODULE hModule = static_cast<HMODULE>(lpReserved);

#ifdef _DEBUG //If in debug version enable console.
    AllocConsole();
    freopen_s(&pStdIn, "CONIN$", "r", stdin);
    freopen_s(&pStdOut, "CONOUT$", "w", stdout);
    freopen_s(&pStdErr, "CONOUT$", "w", stderr);
    SetConsoleTitleW(L"Brick Rigs Server Director");
    SetConsoleOutputCP(CP_UTF8);
#endif // _DEBUG

    MH_Initialize(); //Initalize MinHook

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST); //Prevent random freezes

    MainLoop();

    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();

#ifdef _DEBUG
    fclose(pStdIn);
    fclose(pStdOut);
    fclose(pStdErr);
    SetStdHandle(STD_INPUT_HANDLE, nullptr);
    SetStdHandle(STD_OUTPUT_HANDLE, nullptr);
    SetStdHandle(STD_ERROR_HANDLE, nullptr);
    FreeConsole();
    PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);
#endif 

    FreeLibraryAndExitThread(hModule, 0);
    return 0;

}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
        break;
    }
    return TRUE;
}

