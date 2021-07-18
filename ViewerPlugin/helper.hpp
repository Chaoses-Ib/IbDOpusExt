#pragma once
#include <string>
#include <optional>
#include <functional>
#include <detours/detours.h>
#include <xmmintrin.h>
#include <IbWinCppLib/WinCppLib.hpp>

using namespace std;
using namespace ib;

void DebugOutput(const wchar* str);
void DebugOutput(const wstring &str);
void DebugOutput(const wstringstream &str);

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