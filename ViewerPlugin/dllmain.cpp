#include "pch.h"
#include <string>
#include "DOpus.hpp"

using namespace std;

void DebugOutput(wstring str) {
#ifdef _DEBUG
    OutputDebugStringW((L"Ib" + str).c_str());
#endif // _DEBUG
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

BOOL DVP_InitEx(LPDVPINITEXDATA pInitExData) {
    DebugOutput(L"DVP_InitEx");
    return TRUE;
}

void DVP_Uninit(void) {
    DebugOutput(L"DVP_InitEx");
    return;
}

BOOL DVP_USBSafe(LPOPUSUSBSAFEDATA pUSBSafeData) {
    DebugOutput(L"DVP_USBSafe");
    return TRUE;
}