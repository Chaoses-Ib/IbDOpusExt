#pragma once
#include <string>
#include <sstream>
#include <regex>
using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

#include <boost/di.hpp>
#include <boost/di/extension/scopes/shared.hpp>
namespace di = boost::di;

#include <IbEverythingLib/Everything.hpp>

#include "DOpus.hpp"
#include "GuiShell.hpp"

namespace DOpusExt {
    using namespace DOpus;

    class Main {
        di::injector<di::extension::shared_config> injector;
    public:
        gui::ConfigData config;

        Main();
        Main(gui::ConfigData&&);
        ~Main();
    };

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
            auto ends_with = [](std::wstring_view str, std::wstring_view suffix)
            {
                return str.size() >= suffix.size() && !str.compare(str.size() - suffix.size(), suffix.size(), suffix);
            };

            if (hook
                && fInfoLevelId == FindExInfoStandard && fSearchOp == FindExSearchNameMatch && dwAdditionalFlags == FIND_FIRST_EX_LARGE_FETCH
                && ends_with(lpFileName, L"\\*"))
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
#ifdef _DEBUG
                static fs::path last_parent;  //there may be multiple threads querying different folders at the same time
                static QueryResults results;
#else
                thread_local fs::path last_parent;  //there may be multiple threads querying different folders at the same time
                thread_local QueryResults results;
#endif
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
                for (DWORD i = 0; i < results.available_num; i++) {
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
            enable = false;
            IbDetourDetach(&FindFirstFileExW_real, FindFirstFileExW_detour);
            //IbDetourDetach(&FindNextFileW_Real, FindNextFileW_Detour);
            //IbDetourDetach(&FindClose_Real, FindClose_Detour);
        }
    };

    class CommandExt {
        Command::EventExecuteCommands& event_execute_commands;
        decltype(event_execute_commands.before)::Handle event_execute_commands_handle;
        Command::EventShellExecute& event_shell_execute;
        decltype(event_shell_execute.before)::Handle event_shell_execute_handle;
    public:
        CommandExt(
            Command::EventExecuteCommands& event_execute_commands,
            Command::EventShellExecute& event_shell_execute,
            Thumbnails::MaxSize& thumb_max_size
        ) : event_execute_commands(event_execute_commands), event_shell_execute(event_shell_execute)
        {
            event_execute_commands_handle = event_execute_commands.before.append([&](Command::CommandContainer* cmds) {
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

            event_shell_execute_handle = event_shell_execute.before.append([&](SHELLEXECUTEINFOW* pExecInfo, int& ignore) {
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

        ~CommandExt()
        {
            event_execute_commands.before.remove(event_execute_commands_handle);
            event_shell_execute.before.remove(event_shell_execute_handle);
        }
    };

    class MaxUndoNum {
        Modules::dopus& dopus;
    public:
        MaxUndoNum(Main& main, Modules::dopus& dopus) : dopus(dopus) {
            mem::protect_changed(dopus.base.offset(0x8022C6 + 5), 1, mem::Protect::Write, [&main](Addr p) {
                *(Byte*)p = main.config.FileOperations.Logging.MaxUndoNum;
            });
        }
        ~MaxUndoNum() {
            mem::protect_changed(dopus.base.offset(0x8022C6 + 5), 1, mem::Protect::Write, [](Addr p) {
                *(Byte*)p = 10;
            });
        }
    };


    template<typename T>
    struct Require {
        Require(T&) {}
    };

    inline Main::Main(gui::ConfigData&& config_)
        : config(std::move(config_)),
        injector(di::make_injector<di::extension::shared_config>(
            di::bind<Main>.to(*this)
        ))
    {
        injector.create<Require<CommandExt>>();

        if (config.FileOperations.Logging.MaxUndoNum_enable) {
            injector.create<Require<MaxUndoNum>>();
        }
        if (config.Folders.FolderBehaviour.GetSizesByEverything) {
            GetSizesByEverything::enable = true;
            injector.create<Require<GetSizesByEverything>>();
        } else {
            GetSizesByEverything::enable = false;
        }
    }

    inline Main::Main()
        : Main(gui::Config::GetConfig()) {}

    inline Main::~Main() {}
}