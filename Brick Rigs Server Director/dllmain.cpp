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

#include <windows.h>
#include "stdio.h"
#include "main.h"
#include <MinHook.h>


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

