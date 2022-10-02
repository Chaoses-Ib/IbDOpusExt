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

#include <IbEverything/Everything.hpp>

#include "DOpus.hpp"
#include "GuiShell.hpp"
#include "helper.hpp"

namespace DOpusExt {
    using namespace DOpus;

    class VFileGetFolderSize {
        static decltype(&FindFirstFileW) FindFirstFileW_real;
        static HANDLE WINAPI FindFirstFileW_detour(
            _In_ LPCWSTR lpFileName,
            _Out_ LPWIN32_FIND_DATAW lpFindFileData
        );

        /*
        static decltype(&FindFirstFileExW) FindFirstFileExW_real = FindFirstFileExW;
        static HANDLE WINAPI FindFirstFileExW_detour(
            _In_ LPCWSTR lpFileName,
            _In_ FINDEX_INFO_LEVELS fInfoLevelId,
            _Out_writes_bytes_(sizeof(WIN32_FIND_DATAW)) LPVOID lpFindFileData,
            _In_ FINDEX_SEARCH_OPS fSearchOp,
            _Reserved_ LPVOID lpSearchFilter,
            _In_ DWORD dwAdditionalFlags
        );

        static decltype(&FindNextFileW) FindNextFileW_Real = FindNextFileW;
        static BOOL WINAPI FindNextFileW_Detour(
                _In_ HANDLE hFindFile,
                _Out_ LPWIN32_FIND_DATAW lpFindFileData
        );

        static decltype(&FindClose) FindClose_Real = FindClose;
        static BOOL WINAPI FindClose_Detour(_Inout_ HANDLE hFindFile);
        */
    public:
        VFileGetFolderSize();
        //VFileGetFolderSize(const VFileGetFolderSize&) = delete;
        //VFileGetFolderSize& operator=(VFileGetFolderSize const&) = delete;

        ~VFileGetFolderSize();
    };

    class Main {
        di::core::injector<di::extension::shared_config, di::core::pool_t<>> injector;
        VFileGetFolderSize VFileGetFolderSize_;
    public:
        gui::ConfigData config;

        Main();
        Main(gui::ConfigData&&);
        ~Main();
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
        );

        ~CommandExt();
    };

    class MaxUndoNum {
        Modules::dopus& dopus;
    public:
        MaxUndoNum(Main& main, Modules::dopus& dopus);
        ~MaxUndoNum();
    };


    template<typename T>
    struct Require {
        Require(T&) {}
    };
}