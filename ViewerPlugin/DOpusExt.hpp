#pragma once
#include <string>
#include <sstream>
#include <regex>
#include <unordered_map>
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

    class VFileGetFolderSize {
        static inline decltype(&FindFirstFileW) FindFirstFileW_real = FindFirstFileW;
        static inline HANDLE WINAPI FindFirstFileW_detour(
            _In_ LPCWSTR lpFileName,
            _Out_ LPWIN32_FIND_DATAW lpFindFileData
        )
        {
            wstring_view filename(lpFileName);
            if (filename.rfind(LR"(V:\Ib\GetFolderSize\)", 0) == 0) {
                auto ends_with = [](std::wstring_view str, std::wstring_view suffix)
                {
                    return str.size() >= suffix.size() && !str.compare(str.size() - suffix.size(), suffix.size(), suffix);
                };
                if (!ends_with(filename, LR"(\<IbVFileEnd>)")) {
                    //reduce time: 18us -> 5us
                    return INVALID_HANDLE_VALUE;
                }

                /*
                wstringstream ss;
                ss << filename[std::size(LR"(V:\Ib\GetFolderSize\)") - 1] << L':' << filename.substr(std::size(LR"(V:\Ib\GetFolderSize\)") - 1 + 1);
                wstring filename_arg = ss.str();
                */
                wstring filename_arg{
                    filename.substr(
                        LR"(V:\Ib\GetFolderSize\)"sv.size(),
                        filename.size() - LR"(V:\Ib\GetFolderSize\)"sv.size() - LR"(\<IbVFileEnd>)"sv.size()
                    )
                };

                if constexpr (ib::debug_runtime)
                    DebugOStream() << L"VFileGetFolderSize: " << lpFileName
                    << L", " << filename_arg
                    << L" (thread " << this_thread::get_id() << L")" << std::endl;

                WIN32_FIND_DATAW* find = Addr(lpFindFileData);
                find->dwFileAttributes = FILE_ATTRIBUTE_NORMAL;

                using namespace Everythings;
                static EverythingMT ev;
                wstring parent(filename_arg);
                PathRemoveFileSpecW(parent.data());  //or PathCchRemoveFileSpec
                parent.resize(wcslen(parent.data()));

                thread_local wstring last_parent;  //there may be multiple threads querying different folders at the same time
                thread_local QueryResults results;
                thread_local unordered_map<wstring_view, uint64_t> result_map;

                if (parent != last_parent) {
                    if constexpr (ib::debug_runtime)
                        DebugOStream() << L"VFileGetFolderSize: " << (LR"(folder:infolder:")"s + ib::get_realpath(parent) + L'"')
                        << L" (thread " << this_thread::get_id() << L")" << std::endl;

                    result_map.clear();
                    std::future<QueryResults> fut = ev.query_send(
                        LR"(folder:infolder:")"s + ib::get_realpath(parent) + L'"',
                        0,
                        Request::FileName | Request::Size
                    );
                    std::future_status status = fut.wait_for(std::chrono::milliseconds(3000));
                    if (status == std::future_status::timeout)
                        results.available_num = 0;
                    else
                        results = fut.get();
                    last_parent = parent;
                    
                    for (DWORD i = 0; i < results.available_num; i++) {
                        result_map[results[i].get_str(Request::FileName)] = results[i].get_size();
                    }
                }

                uint64_t size = 0;
                if (results.available_num) {
                    auto it = result_map.find(PathFindFileNameW(filename_arg.data()));
                    if (it != result_map.end()) {
                        size = it->second;

                        if (!size) {
                            wstring realpath = ib::get_realpath(filename_arg);
                            if (realpath != filename_arg) {
                                if constexpr (ib::debug_runtime)
                                    DebugOStream() << L"VFileGetFolderSize: " << (LR"(wfn:")"s + realpath + L'"')
                                    << L" (thread " << this_thread::get_id() << L")" << std::endl;
                                
                                std::future<QueryResults> fut = ev.query_send(
                                    LR"(wfn:")"s + realpath + L'"',
                                    0,
                                    Request::Size
                                );
                                std::future_status status = fut.wait_for(std::chrono::milliseconds(3000));

                                QueryResults results;
                                if (status == std::future_status::timeout)
                                    results.available_num = 0;
                                else
                                    results = fut.get();
                                
                                if (results.available_num)
                                    size = results[0].get_size();
                                else
                                    ;  //ignore
                            }
                        }
                    }
                    else
                        ;  //ignore
                }

                find->nFileSizeLow = (DWORD)size;
                find->nFileSizeHigh = (DWORD)(size >> 32);
                if constexpr (ib::debug_runtime)
                    DebugOStream() << L"VFileGetFolderSize: " << size << std::endl;

                SetLastError(ERROR_SUCCESS);  //can't be ERROR_ACCESS_DENIED
                return (HANDLE)1;  //can't be 0
            }
            return FindFirstFileW_real(lpFileName, lpFindFileData);
        }

        /*
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
            return FindFirstFileExW_real(lpFileName, fInfoLevelId, lpFindFileData, fSearchOp, lpSearchFilter, dwAdditionalFlags);
        }

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
        VFileGetFolderSize() {
            IbDetourAttach(&FindFirstFileW_real, FindFirstFileW_detour);
            //#TODO: transaction
            //IbDetourAttach(&FindFirstFileExW_real, FindFirstFileExW_detour);
            //IbDetourAttach(&FindNextFileW_Real, FindNextFileW_Detour);
            //IbDetourAttach(&FindClose_Real, FindClose_Detour);
        }

        ~VFileGetFolderSize() {
            IbDetourDetach(&FindFirstFileW_real, FindFirstFileW_detour);
            //IbDetourDetach(&FindFirstFileExW_real, FindFirstFileExW_detour);
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
                DebugOStream() << L"START ----------------" << cmds->size << L" (" << HeapSize(GetProcessHeap(), 0, cmds) << L")"
                    << L" (thread " << this_thread::get_id() << L")" << std::endl;
                Command::CommandNode* node = cmds->head;
                do {
                    wchar* cmd = node->command;
                    if (!cmd) {
                        //DebugOStream() << L" (" << HeapSize(GetProcessHeap(), 0, node) << L")") << std::endl;
                        continue;
                    }
                    DebugOStream() << cmd << L" (" << HeapSize(GetProcessHeap(), 0, node) << L")" << std::endl;

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
                            DebugOStream() << L"-> " << cmd << std::endl;
                    }
                } while (node = node->next);
                DebugOStream() << L"END ----------------" << std::endl;
            });

            event_shell_execute_handle = event_shell_execute.before.append([&](SHELLEXECUTEINFOW* pExecInfo, int& ignore) {
                if (!pExecInfo->lpFile)
                    return;
                DebugOStream() << L"ShellExecute: " << (pExecInfo->lpFile ? pExecInfo->lpFile : L"") << L" " << (pExecInfo->lpParameters ? pExecInfo->lpParameters : L"") << std::endl;

                wstring_view file = pExecInfo->lpFile ? wstring_view(pExecInfo->lpFile) : wstring_view();
                wstring_view params = pExecInfo->lpParameters ? wstring_view(pExecInfo->lpParameters) : wstring_view();
                if (file[0] == L'#') {
                    file = file.substr(1);
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
        injector.create<Require<VFileGetFolderSize>>();

        if (config.FileOperations.Logging.MaxUndoNum_enable) {
            injector.create<Require<MaxUndoNum>>();
        }
    }

    inline Main::Main()
        : Main(gui::Config::GetConfig()) {}

    inline Main::~Main() {}
}