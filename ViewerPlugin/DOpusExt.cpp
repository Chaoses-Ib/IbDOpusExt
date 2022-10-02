#include "pch.h"
#include "DOpusExt.hpp"

namespace DOpusExt {
    decltype(&FindFirstFileW) VFileGetFolderSize::FindFirstFileW_real = FindFirstFileW;
    
    HANDLE __stdcall VFileGetFolderSize::FindFirstFileW_detour(LPCWSTR lpFileName, LPWIN32_FIND_DATAW lpFindFileData)
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

            uint64_t size = 0;
            if (filename_arg.size() == 3) {  //R"(C:\)"
                GetDiskFreeSpaceExW(filename_arg.c_str(), nullptr, ib::Addr(&size), nullptr);
            }
            else {
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
                        DebugOStream() << L"VFileGetFolderSize: " << (LR"(folder:infolder:")"s + ib::path_to_realpath(parent) + L'"')
                        << L" (thread " << this_thread::get_id() << L")" << std::endl;

                    result_map.clear();
                    std::future<QueryResults> fut = ev.query_send(
                        LR"(folder:infolder:")"s + ib::path_to_realpath(parent) + L'"',
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

                if (results.available_num) {
                    auto it = result_map.find(PathFindFileNameW(filename_arg.data()));
                    if (it != result_map.end()) {
                        size = it->second;

                        if (!size) {
                            wstring realpath = ib::path_to_realpath(filename_arg);
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

    VFileGetFolderSize::VFileGetFolderSize() {
        if constexpr (debug)
            DebugOStream() << "VFileGetFolderSize()\n";

        //#TODO
        static bool init = false;
        if (!init) {
            IbDetourAttach(&FindFirstFileW_real, FindFirstFileW_detour);
            init = true;
        }

        //#TODO: transaction
        //IbDetourAttach(&FindFirstFileExW_real, FindFirstFileExW_detour);
        //IbDetourAttach(&FindNextFileW_Real, FindNextFileW_Detour);
        //IbDetourAttach(&FindClose_Real, FindClose_Detour);
    }

    VFileGetFolderSize::~VFileGetFolderSize() {
        if constexpr (debug)
            DebugOStream() << "~VFileGetFolderSize()\n";
        //#TODO
        //IbDetourDetach(&FindFirstFileW_real, FindFirstFileW_detour);
        
        //IbDetourDetach(&FindFirstFileExW_real, FindFirstFileExW_detour);
        //IbDetourDetach(&FindNextFileW_Real, FindNextFileW_Detour);
        //IbDetourDetach(&FindClose_Real, FindClose_Detour);
    }

    /*
    HANDLE __stdcall VFileGetFolderSize::FindFirstFileExW_detour(LPCWSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags)
    {
        return FindFirstFileExW_real(lpFileName, fInfoLevelId, lpFindFileData, fSearchOp, lpSearchFilter, dwAdditionalFlags);
    }

    BOOL __stdcall VFileGetFolderSize::FindNextFileW_Detour(HANDLE hFindFile, LPWIN32_FIND_DATAW lpFindFileData)
    {
        if (hFindFile == 0) {
            SetLastError(ERROR_NO_MORE_FILES);
            return FALSE;
        }
        return FindNextFileW_Real(hFindFile, lpFindFileData);
    }

    BOOL __stdcall VFileGetFolderSize::FindClose_Detour(HANDLE hFindFile)
    {
        if (hFindFile == 0) {
            return TRUE;
        }
        return FindClose_Real(hFindFile);
    }
    */

    CommandExt::CommandExt(
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

    CommandExt::~CommandExt()
    {
        event_execute_commands.before.remove(event_execute_commands_handle);
        event_shell_execute.before.remove(event_shell_execute_handle);
    }

    MaxUndoNum::MaxUndoNum(Main& main, Modules::dopus& dopus) : dopus(dopus) {
        if (dvp_init_data.dwOpusVerMajor == 12 && dvp_init_data.dwOpusVerMinor == 23) {
            DebugOStream() << L"MaxUndoNum: " << main.config.FileOperations.Logging.MaxUndoNum << std::endl;

            mem::protect_changed(dopus.base.offset(0x8022C6 + 5), 1, mem::Protect::Write, [&main](Addr p) {
                *(Byte*)p = main.config.FileOperations.Logging.MaxUndoNum;
                });
        }
    }

    MaxUndoNum::~MaxUndoNum() {
        if (dvp_init_data.dwOpusVerMajor == 12 && dvp_init_data.dwOpusVerMinor == 23) {
            mem::protect_changed(dopus.base.offset(0x8022C6 + 5), 1, mem::Protect::Write, [](Addr p) {
                *(Byte*)p = 10;
                });
        }
    }

    Main::Main(gui::ConfigData&& config_)
        : config(std::move(config_)),
        injector(di::make_injector<di::extension::shared_config>(
            di::bind<Main>.to(*this)
        ))
    {
        injector.create<Require<CommandExt>>();
        VFileGetFolderSize_ = injector.create<VFileGetFolderSize>();

        if (config.FileOperations.Logging.MaxUndoNum_enable) {
            injector.create<Require<MaxUndoNum>>();
        }
    }

    Main::Main()
        : Main(gui::Config::GetConfig()) {}

    Main::~Main() {}
}