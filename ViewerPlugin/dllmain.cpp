#include "pch.h"
#include <string>
#include <sstream>
#include <regex>
#include <boost/di.hpp>
#include <yaml-cpp/yaml.h>
#include "DOpusPlugin.hpp"
#include "helper.hpp"
#include "DOpus.hpp"

using namespace std;
namespace di = boost::di;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DebugOutput(L"DLL_PROCESS_ATTACH");
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        DebugOutput(L"DLL_PROCESS_DETACH");
        break;
    }
    return TRUE;
}

#define EXPORT extern "C" __declspec(dllexport)

namespace DOpusExt {
    using namespace DOpus;

    class LogCommands {
    public:
        LogCommands(Command::EventExecuteCommands& event_execute_commands) {
            event_execute_commands.callbacks.append([](Command::EventExecuteCommands::Command* cmd, uint64_t n) {
                DebugOutput(L"START ----------------"s + to_wstring(n));
                do {
                    if (cmd->Command) {
                        DebugOutput(cmd->Command);
                    }
                } while (cmd = cmd->Next);
                DebugOutput(L"END ----------------");
            });
        }
    };

    class CommandExt {
    public:
        CommandExt(Command::EventExecuteCommands& event_execute_commands, Mem& mem,
            Thumbnails::MaxSize& thumb_max_size) {
            event_execute_commands.callbacks.append([&](Command::EventExecuteCommands::Command* cmd, uint64_t n) {
                bool ibext = false;
                do {
                    if (!cmd->Command) continue; DebugOutput(cmd->Command);
                    if (!_wcsicmp(cmd->Command, L"@ibext")) {
                        ibext = true;
                        cmd->Command[0] = L'\0';
                        cmd = cmd->Next;
                        break;
                    }
                } while (cmd = cmd->Next);
                if (!ibext) return;
                do{
                    if (!cmd->Command) continue; DebugOutput(cmd->Command);

                    //Set MaxThumbSize
                    static wregex reg(LR"(^ *Set +MaxThumbSize *= *(\d+))", regex_constants::icase);
                    wcmatch match;
                    if (regex_search(cmd->Command, match, reg) && match.size() > 1) {
                        uint32_t size;
                        wstringstream() << match.str(1) >> size;
                        thumb_max_size.Set(size);
                        wcscpy_s(cmd->Command, 11, L"Go Refresh");  //>=18
                    }

                } while (cmd = cmd->Next);
            });
        }
    };

    class Main {
    public:
        Main(
#ifdef _DEBUG
            LogCommands&,
#endif
            CommandExt&
        ) { }
    };
}

static optional<DOpusExt::Main> Ext;

EXPORT BOOL DVP_InitEx(DVPInitExData pInitExData) {
    DebugOutput(L"DVP_InitEx");
    if (!Ext) {
        Ext = di::make_injector().create<DOpusExt::Main>();
    }
    return TRUE;
}

EXPORT void DVP_Uninit(void) {
    DebugOutput(L"DVP_Uninit");

    //In order to be autoloaded and still can be unloaded by "Show flushplugins".
    static bool first_time = true;
    static HMODULE handle;
    if (first_time) {
        GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)DVP_Uninit, &handle);
        first_time = false;
    }
    else if(handle) {
        FreeLibrary(handle);
        handle = nullptr;
    }
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
    lpVPInfo->dwFlags = DVPFIF_ExtensionsOnly  //or DVPFIF_CatchAll
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