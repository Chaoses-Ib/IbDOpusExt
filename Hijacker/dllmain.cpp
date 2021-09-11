#include "pch.h"
#include <detours/detours.h>
#include <IbWinCppLib/WinCppLib.hpp>

template<typename T>
LONG IbDetourAttach(_Inout_ T* ppPointer, _In_ T pDetour) {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach((void**)ppPointer, pDetour);
    return DetourTransactionCommit();
}

template<typename T>
LONG IbDetourDetach(_Inout_ T* ppPointer, _In_ T pDetour) {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach((void**)ppPointer, pDetour);
    return DetourTransactionCommit();
}

decltype(&LoadLibraryW) LoadLibraryW_true = LoadLibraryW;
_Ret_maybenull_ HMODULE WINAPI LoadLibraryW_detour(_In_ LPCWSTR lpLibFileName)
{
    if constexpr (ib::debug_runtime) {
        ib::DebugOStream() << L"LoadLibraryW: " << lpLibFileName << std::endl;
        HMODULE result;
        if (lpLibFileName[0] != L'\0' && lpLibFileName[1] == L':')
            result = LoadLibraryExW(lpLibFileName, 0, LOAD_WITH_ALTERED_SEARCH_PATH);
        else
            result = LoadLibraryW_true(lpLibFileName);
        DWORD error = GetLastError();
        ib::DebugOStream() << result << ", " << error << std::endl;
    }

    if (lpLibFileName[0] != L'\0' && lpLibFileName[1] == L':')
        return LoadLibraryExW(lpLibFileName, 0, LOAD_WITH_ALTERED_SEARCH_PATH);
    else
        return LoadLibraryW_true(lpLibFileName);
}

#include <IbDllHijackLib/Dlls/version.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if constexpr (ib::debug_runtime) {
        const wchar_t* reason_table[] = { L"DLL_PROCESS_DETACH", L"DLL_PROCESS_ATTACH", L"DLL_THREAD_ATTACH", L"DLL_THREAD_DETACH" };
        ib::DebugOStream() << "DllMain: " << reason_table[ul_reason_for_call] << std::endl;
    }

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        IbDetourAttach(&LoadLibraryW_true, LoadLibraryW_detour);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}