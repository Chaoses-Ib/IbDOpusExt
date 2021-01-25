#include "pch.h"
#include <string>
#include "DOpus.hpp"

using namespace std;

void DebugOutput(wstring str) {
#ifdef _DEBUG
    OutputDebugStringW((L"Ib: " + str).c_str());
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

#define EXPORT extern "C" __declspec(dllexport) 

EXPORT BOOL DVP_InitEx(DVPInitExData pInitExData) {
    DebugOutput(L"DVP_InitEx");
    return TRUE;
}

EXPORT void DVP_Uninit(void) {
    DebugOutput(L"DVP_Uninit");
    return;
}

EXPORT BOOL DVP_USBSafe(OpusUSBSafeData* pUSBSafeData) {
    DebugOutput(L"DVP_USBSafe");
    return TRUE;
}

EXPORT BOOL DVP_IdentifyW(DOpusViewerPluginInfoW* lpVPInfo) {
    DebugOutput(L"DVP_IdentifyW");
    if (lpVPInfo->cbSize < VIEWERPLUGINFILEINFOW_V4_SIZE)
        return FALSE;
    lpVPInfo->dwFlags = DVPFIF_ExtensionsOnly
                      | DVPFIF_NoThumbnails
                      | DVPFIF_NoFileInformation
                      | DVPFIF_UseVersionResource  //only for version and copyright
                      ;
    //wcscpy_s(lpVPInfo->lpszHandleExts, lpVPInfo->cchHandleExtsMax, L".abc");

    wcscpy_s(lpVPInfo->lpszName, lpVPInfo->cchNameMax, L"IbDOpusExt");
    wcscpy_s(lpVPInfo->lpszDescription, lpVPInfo->cchDescriptionMax, L"An extension for DOpus.");
    wcscpy_s(lpVPInfo->lpszURL, lpVPInfo->cchURLMax, L"https://github.com/Chaoses-Ib/IbDOpusExt");

    // {CB2D22A9-6C03-4B5D-B264-60224E70A46F}
    lpVPInfo->idPlugin = { 0xcb2d22a9, 0x6c03, 0x4b5d, { 0xb2, 0x64, 0x60, 0x22, 0x4e, 0x70, 0xa4, 0x6F } };

    return TRUE;
}