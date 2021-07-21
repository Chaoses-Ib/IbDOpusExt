#include "pch.h"
#include <string>
#include <sstream>
#include <regex>

#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

#include <boost/di.hpp>
#include <yaml-cpp/yaml.h>
#include "DOpusPlugin.hpp"
#include "helper.hpp"
#include "DOpus.hpp"
#include <IbEverythingLib/Everything.hpp>

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

    class GetSizesByEverything {
        static inline decltype(&FindFirstFileExW) FindFirstFileExW_real = FindFirstFileExW;
        static inline HANDLE WINAPI FindFirstFileExW_detour(
            _In_ LPCWSTR lpFileName,
            _In_ FINDEX_INFO_LEVELS fInfoLevelId,
            _Out_writes_bytes_(sizeof(WIN32_FIND_DATAW)) LPVOID lpFindFileData,
            _In_ FINDEX_SEARCH_OPS fSearchOp,
            _Reserved_ LPVOID lpSearchFilter,
            _In_ DWORD dwAdditionalFlags
        )
        {
            if (hook
                && fInfoLevelId == FindExInfoStandard && fSearchOp == FindExSearchNameMatch && dwAdditionalFlags == FIND_FIRST_EX_LARGE_FETCH
                && wstring_view(lpFileName).ends_with(L"\\*"))
            {
                //FindClose(FindFirstFileExW_real(lpFileName, fInfoLevelId, lpFindFileData, fSearchOp, lpSearchFilter, dwAdditionalFlags));

                WIN32_FIND_DATAW* find = Addr(lpFindFileData);
                find->dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
                wcscpy_s(find->cFileName, L"#GetSizesByEverything");

                using namespace Everythings;
                static EverythingMT ev;
                fs::path path(lpFileName);
                fs::path folder = path.parent_path();
                fs::path parent = folder.parent_path();
                thread_local fs::path last_parent;  //there may be multiple threads querying different folders at the same time
                thread_local QueryResults results;
                if (parent != last_parent) {
                    DebugOutput(wstringstream() << L"GetSizesByEverything: " << (LR"(folder:infolder:")"s + get_realpath(parent.wstring()) + L'"')
                        << L" (thread " << this_thread::get_id() << L")");
                    results = ev.query_send(
                        LR"(folder:infolder:")"s + get_realpath(parent.wstring()) + L'"',
                        0,
                        Request::FileName | Request::Size
                    ).get();
                    last_parent = parent;
                }
                
                uint64_t size = 0;
                find->nFileSizeHigh = find->nFileSizeLow = 0;
                for (DWORD i = 0; i < results.query_num; i++) {
                    if (results[i].get_str(Request::FileName) == folder.filename().wstring()) {
                        size = results[i].get_size();
                        if (!size) {
                            wstring realpath = get_realpath(folder.wstring());
                            if (realpath != folder.wstring()) {
                                DebugOutput(wstringstream() << L"GetSizesByEverything: " << (LR"(wfn:")"s + realpath + L'"')
                                    << L" (thread " << this_thread::get_id() << L")");
                                QueryResults results2 = ev.query_send(
                                    LR"(wfn:")"s + realpath + L'"',
                                    0,
                                    Request::Size
                                ).get();
                                if (results2.found_num)
                                    size = results2[0].get_size();
                                //ignore
                            }
                            //ignore
                        }
                        break;
                    }
                }
                find->nFileSizeLow = (DWORD)size;
                find->nFileSizeHigh = (DWORD)(size >> 32);

                DebugOutput(wstringstream() << L"GetSizesByEverything: " << lpFileName << ", " << find->nFileSizeLow
                    << L" (thread " << this_thread::get_id() << L")");
                return 0;
            }
            return FindFirstFileExW_real(lpFileName, fInfoLevelId, lpFindFileData, fSearchOp, lpSearchFilter, dwAdditionalFlags);
        }

        /*
        static inline decltype(&FindNextFileW) FindNextFileW_Real = FindNextFileW;
        static inline BOOL WINAPI FindNextFileW_Detour(
                _In_ HANDLE hFindFile,
                _Out_ LPWIN32_FIND_DATAW lpFindFileData
            )
        {
            if (hFindFile == 0) {
                SetLastError(ERROR_NO_MORE_FILES);
                return FALSE;
            }
            return FindNextFileW_Real(hFindFile, lpFindFileData);
        }

        static inline decltype(&FindClose) FindClose_Real = FindClose;
        static inline BOOL WINAPI FindClose_Detour(_Inout_ HANDLE hFindFile)
        {
            if (hFindFile == 0) {
                return TRUE;
            }
            return FindClose_Real(hFindFile);
        }
        */
    public:
        static inline bool hook = false;
        static inline bool enable = false;

        GetSizesByEverything() {
            //#TODO: transaction
            IbDetourAttach(&FindFirstFileExW_real, FindFirstFileExW_detour);
            //IbDetourAttach(&FindNextFileW_Real, FindNextFileW_Detour);
            //IbDetourAttach(&FindClose_Real, FindClose_Detour);
        }

        ~GetSizesByEverything() {
            IbDetourDetach(&FindFirstFileExW_real, FindFirstFileExW_detour);
            //IbDetourDetach(&FindNextFileW_Real, FindNextFileW_Detour);
            //IbDetourDetach(&FindClose_Real, FindClose_Detour);
        }
    };

    class CommandExt {
    public:
        CommandExt(
            Command::EventExecuteCommands& event_execute_commands,
            Command::EventShellExecute& event_shell_execute,
            Thumbnails::MaxSize& thumb_max_size
        ){
            event_execute_commands.before.append([&](Command::CommandContainer* cmds) {
                DebugOutput(wstringstream() << L"START ----------------" << cmds->size << L" (" << HeapSize(GetProcessHeap(), 0, cmds) << L")"
                    << L" (thread " << this_thread::get_id() << L")");
                Command::CommandNode* node = cmds->head;
                do {
                    wchar* cmd = node->command;
                    if (!cmd) {
                        //DebugOutput(wstringstream() << L" (" << HeapSize(GetProcessHeap(), 0, node) << L")");
                        continue;
                    }
                    DebugOutput(wstringstream() << cmd << L" (" << HeapSize(GetProcessHeap(), 0, node) << L")");

                    //GetSizesByEverything
                    if (GetSizesByEverything::enable && !GetSizesByEverything::hook) {
                        if (!_wcsicmp(cmd, L"GetSizes")) {
                            GetSizesByEverything::hook = true;
                            cmds->push_back(L"#GetSizesByEverything _End");
                        }
                    }

                    if (cmd[0] == L'#') {
                        do {
                            //Set MaxThumbSize
                            static wregex reg(LR"(^ *Set +MaxThumbSize *= *(\d+))", regex_constants::icase);
                            wcmatch match;
                            if (regex_search(cmd + 1, match, reg) && match.size() > 1) {
                                uint32_t size;
                                wstringstream() << match.str(1) >> size;
                                thumb_max_size.Set(size);
                                wcsncpy_s(cmd, std::size(L"Go Refresh"), L"Go Refresh", std::size(L"Go Refresh"));  //>=18
                                break;
                            }

                            //cmd[0] = L'\0';
                        } while (false);

                        if (cmd[0] != L'\0')
                            DebugOutput(L"-> "s + cmd);
                    }
                } while (node = node->next);
                DebugOutput(L"END ----------------");
            });

            event_shell_execute.before.append([&](SHELLEXECUTEINFOW* pExecInfo, int& ignore) {
                if (!pExecInfo->lpFile)
                    return;
                DebugOutput(L"ShellExecute: "s + (pExecInfo->lpFile ? pExecInfo->lpFile : L"") + L" " + (pExecInfo->lpParameters ? pExecInfo->lpParameters : L""));
                
                wstring_view file = pExecInfo->lpFile ? wstring_view(pExecInfo->lpFile) : wstring_view();
                wstring_view params = pExecInfo->lpParameters ? wstring_view(pExecInfo->lpParameters) : wstring_view();
                if (file[0] == L'#') {
                    file = file.substr(1);
                    if (file == L"GetSizesByEverything" && !params.empty()) {
                        if (params == L"On") {
                            GetSizesByEverything::enable = true;
                        }
                        else if (params == L"Off") {
                            GetSizesByEverything::enable = false;
                        }
                        else if (params == L"Toggle") {
                            GetSizesByEverything::enable = !GetSizesByEverything::enable;
                        }
                        else if (params == L"_End") {
                            GetSizesByEverything::hook = false;
                        }
                        ignore = 1;
                    }
                }
            });
        }
    };

    class Main {
    public:
        Main(
            Modules::dopus& dopus,
            CommandExt&,
            GetSizesByEverything&
        )
        {
            using namespace std::filesystem;
            path ext_path = [] {
                DOpusPluginHelperConfig opusconfig;
                wchar program_dir[MAX_PATH];
                opusconfig.GetConfigPath(OPUSPATH_CONFIG, program_dir, size(program_dir));
                return path(program_dir) / L"IbDOpusExt";
            }();

            YAML::Node yaml;
            path config_path = ext_path / L"config.yaml";

            if (exists(config_path)) {
                auto file = ifstream(config_path);
                istream& s = file;
                yaml = YAML::Load(s);

                if (auto MaxItems = yaml["FileOperations"]["Logging"]["UndoLog"]["MaxItems"]) {
                    uint32_t max_items = MaxItems.as<uint32_t>();
                    mem::protect_changed(dopus.base.offset(0x8022C6 + 5), 1, mem::Protect::Write, [max_items](Addr p) {
                        *(Byte*)p = uint8_t(max_items);
                    });
                }
                if (auto node = yaml["Folders"]["FolderBehaviour"]["GetSizesByEverything"])
                    GetSizesByEverything::enable = node.as<bool>();
            }
        }
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