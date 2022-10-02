#pragma once
#include <string>
#include <optional>
#include <functional>

#include <detours/detours.h>
#include <xmmintrin.h>

#include <IbWinCpp/WinCpp.hpp>
using ib::wchar, ib::Byte, ib::Addr;
namespace mem = ib::mem;

#include "DOpusPlugin.hpp"
extern DVPInitExData dvp_init_data;

constexpr bool debug = !ib::macro::ndebug;

inline ib::DebugOStream<> DebugOStream() {
    return ib::DebugOStream(L"Ib: ");
}


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