﻿#include "pch.h"
#include "DOpusPlugin.hpp"

#include "helper.hpp"
#include "DOpusExt.hpp"
#include "GuiShell.hpp"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DebugOStream() << L"DLL_PROCESS_ATTACH" << std::endl;
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        DebugOStream() << L"DLL_PROCESS_DETACH" << std::endl;
        break;
    }
    return TRUE;
}

#define EXPORT extern "C" __declspec(dllexport)

static ib::HolderB<DOpusExt::Main> dopus_ext;

DVPInitExData dvp_init_data;

void ApplyCallback(gui::ConfigData&& config) {
    dopus_ext.recreate(std::move(config));
}

EXPORT BOOL DVP_InitEx(DVPInitExData* pInitExData) {
    DebugOStream() << L"DVP_InitEx" << std::endl;

    dvp_init_data = *pInitExData;

    if (!dopus_ext.created()) {
        using namespace std::filesystem;
        path ext_path = [] {
            DOpusPluginHelperConfig opusconfig;
            wchar program_dir[MAX_PATH];
            opusconfig.GetConfigPath(OPUSPATH_CONFIG, program_dir, (UINT)size(program_dir));
            return path(program_dir) / L"IbDOpusExt";
        }();
        gui::Config::ApplyCallback = ApplyCallback;
        gui::Config::Init((ext_path / L"config.xml").wstring());

        dopus_ext.create();
    }
    return TRUE;
}

EXPORT void DVP_Uninit(void) {
    DebugOStream() << L"DVP_Uninit" << std::endl;

    //In order to be autoloaded and still can be unloaded by "Show flushplugins".
    static bool first_time = true;
    static HMODULE handle;
    if (first_time) {
        if constexpr (debug)
            DebugOStream() << L"first_time" << std::endl;
        GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)DVP_Uninit, &handle);
        first_time = false;
    }
    else if(handle) {
        FreeLibrary(handle);
        handle = nullptr;

        if constexpr (debug)
            DebugOStream() << L"dopus_ext.destroy()" << std::endl;
        dopus_ext.destroy();
    }
    return;
}

EXPORT BOOL DVP_USBSafe(OpusUSBSafeData* pUSBSafeData) {
    DebugOStream() << L"DVP_USBSafe" << std::endl;
    return TRUE;
}

EXPORT BOOL DVP_IdentifyW(DOpusViewerPluginInfoW* lpVPInfo) {
    DebugOStream() << L"DVP_IdentifyW" << std::endl;
    if (lpVPInfo->cbSize < VIEWERPLUGINFILEINFOW_V4_SIZE)
        return FALSE;
    lpVPInfo->dwFlags = DVPFIF_ExtensionsOnly  //or DVPFIF_CatchAll
                      | DVPFIF_NoThumbnails
                      | DVPFIF_NoFileInformation
                      | DVPFIF_UseVersionResource  //only for version and copyright
                      | DVPFIF_CanConfigure
                      ;
    //wcscpy_s(lpVPInfo->lpszHandleExts, lpVPInfo->cchHandleExtsMax, L".abc");

    wcscpy_s(lpVPInfo->lpszName, lpVPInfo->cchNameMax, L"IbDOpusExt");
    wcscpy_s(lpVPInfo->lpszDescription, lpVPInfo->cchDescriptionMax, L"An extension for DOpus.");
    wcscpy_s(lpVPInfo->lpszURL, lpVPInfo->cchURLMax, L"https://github.com/Chaoses-Ib/IbDOpusExt");

    // {CB2D22A9-6C03-4B5D-B264-60224E70A46F}
    lpVPInfo->idPlugin = { 0xcb2d22a9, 0x6c03, 0x4b5d, { 0xb2, 0x64, 0x60, 0x22, 0x4e, 0x70, 0xa4, 0x6F } };

    return TRUE;
}

gui::PreferenceShow& preferences() {
    static gui::PreferenceShow preferences;
    return preferences;
}

EXPORT HWND DVP_Configure(HWND hWndParent, HWND hWndNotify, DWORD dwNotifyData) {
    DebugOStream() << L"DVP_Configure" << std::endl;
    preferences().Show();
    return 0;
}