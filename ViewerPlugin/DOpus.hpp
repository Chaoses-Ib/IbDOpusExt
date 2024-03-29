﻿#pragma once
#include "pch.h"
#include <utility>
#include "helper.hpp"
#include <eventpp/callbacklist.h>
#include <IbWinCpp/WinCpp.hpp>
#include <list>
#include "sigmatch.hpp"

namespace DOpus {
    using eventpp::CallbackList;

    namespace Modules {
        class dopus : public ib::Module {
        public:
            dopus() : Module(ib::ModuleFactory::current_process()) {}
        };
    }

    namespace Mem {
        inline void* alloc(size_t size) {
            /*
            if (size == 0)
                size = 1;
            */
            return HeapAlloc(GetProcessHeap(), 0, size);
            //ignore errors
        }

        inline void free(void* block) {
            HeapFree(GetProcessHeap(), 0, block);
            //ignore errors
        }
    }
    /*
    class Mem {
    public:
        //operator new
        static inline void* (*Alloc)(size_t size);
        //_free_base
        static inline void (*Free)(void* block);
        Mem(Modules::dopus& dopus) {
            Alloc = dopus.base.offset(0xE41630);
            Free = dopus.base.offset(0xE89A64);
        }
    };
    */

    class Prefs {
    public:
        Addr base;
        Prefs(Modules::dopus& dopus) : base( *((void**)sigmatch_Prefs()) ) { }
    };

    namespace Thumbnails {
        class MaxSize {
            uint32_t* Pref_MaxThumbSize;
        public:
            MaxSize(Prefs& prefs) {
                Pref_MaxThumbSize = prefs.base.offset(sigmatch_MaxThumbSize());
                //DebugOutput(std::wstringstream() << (void*)prefs.base << L" " << Pref_MaxThumbSize);
            }
            //Will refresh the max range of trackbars, but won't refresh buttons.
            void Set(uint32_t size) {
                uint32_t oldsize = std::exchange(*Pref_MaxThumbSize, size);

                using ib::FindWindowEx_i;
                for (HWND lister : FindWindowEx_i(0, L"dopus.lister")) {
                    for (HWND toolbar : FindWindowEx_i(lister, L"dopus.button.display")) {
                        for (HWND trackbar : FindWindowEx_i(toolbar, L"dopus.trackbar")) {
                            if (SendMessageW(trackbar, TBM_GETRANGEMIN, 0, 0) == 32
                                && SendMessageW(trackbar, TBM_GETRANGEMAX, 0, 0) == oldsize) {
                                SendMessageW(trackbar, TBM_SETRANGEMAX, TRUE, size);
                            }
                        }
                    }
                }
                
            }
            uint32_t Get() {
                return *Pref_MaxThumbSize;
            }
        };
        
    }

    namespace Command{
        struct CommandNode
        {
            //node: { next/end, prior, 0,   0, command, ...1 }
            //end:  {        0, prior, 0, num,       0,    0 }
            CommandNode* next;
            CommandNode* prior;
            uint64_t _2 = 0;
            union {
                uint64_t _3 = 0;
                uint64_t end_size;
            };
            wchar* command;  //may be null
            uint64_t _5 = 1;
        private:
            void size_assert() {
                static_assert(sizeof CommandNode == 48);
            }
        public:
            wstring_view view() {
                if (!command)
                    return {};
                return command;
            }
        };

        //#TODO: std::list?
        struct CommandContainer {
            CommandNode* head;
            uint64_t _1 = 0;
            CommandNode* tail;
            uint64_t _3 = 0;
            size_t size;
            uint64_t _5 = 0;
            uint64_t _6 = 0;
            uint64_t _7 = 0;
        private:
            void size_assert() {
                static_assert(sizeof CommandContainer == 64);
            }
        public:
            //#TODO: iterator

            void set_size(size_t new_size) {
                size = new_size;
                tail->next->end_size = new_size;
            }

            //Return the new node.
            CommandNode* insert_after(CommandNode* pos, wstring_view command) {
                CommandNode* node = Addr(Mem::alloc(sizeof CommandNode));
                    node->next = pos->next,
                    node->prior = pos,
                    node->_2 = node->_3 = 0,
                    node->command = Addr(Mem::alloc(sizeof wchar * (command.size() + 1))),
                    node->_5 = 1;
                wcsncpy_s(node->command, command.size() + 1, command.data(), command.size() + 1);
                pos->next = pos->next->prior = node;
                if (pos == tail)
                    tail = node;
                set_size(size + 1);
                return node;
            }

            void push_back(wstring_view command) {
                insert_after(tail, command);
            }
        };

        class EventExecuteCommands {
            static inline uint64_t RunCommandB4_TrueDetour(__int64 a1, CommandContainer* commands, __int64 a3, __int64 a4, DWORD* a5) {
                before(commands);
                return RunCommandB4_True(a1, commands, a3, a4, a5);  //commands are not executed when it returns
            }
            using RunCommandB4_True_t = decltype(RunCommandB4_TrueDetour);
            static inline RunCommandB4_True_t* RunCommandB4_True;
        public:
            static inline CallbackList<void(CommandContainer* cmd)> before;
            EventExecuteCommands(Modules::dopus& dopus) {
                RunCommandB4_True = sigmatch_RunCommandB4_True();
                IbDetourAttach(&RunCommandB4_True, RunCommandB4_TrueDetour);
            }
            ~EventExecuteCommands() {
                IbDetourDetach(&RunCommandB4_True, RunCommandB4_TrueDetour);
            }
        };

        class EventShellExecute {
            static inline decltype(&ShellExecuteExW) ShellExecuteExW_real = ShellExecuteExW;
            static inline BOOL WINAPI ShellExecuteExW_detour(_Inout_ SHELLEXECUTEINFOW* pExecInfo) {
                /*
                    #	Type	Name	Pre-Call Value	Post-Call Value
	                DWORD	cbSize	112	112
	                ULONG	fMask	SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NO_CONSOLE	SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NO_CONSOLE
	                HWND	hwnd	0x00000000001d0480	0x00000000001d0480
	                LPCTSTR	lpVerb	NULL	NULL
	                LPCTSTR	lpFile	0x0000000004e23290 "#FolderSizeByEverything"	0x0000000004e23290 "#FolderSizeByEverything"
	                LPCTSTR	lpParameters	NULL	NULL
	                LPCTSTR	lpDirectory	0x0000000004b0a500 "C:\Windows\system32"	0x0000000004b0a500 "C:\Windows\system32"
	                int	nShow	SW_SHOWNORMAL	SW_SHOWNORMAL
	                HINSTANCE	hInstApp	0	SE_ERR_FNF
	                void*	lpIDList	NULL	NULL
	                LPCTSTR	lpClass	NULL	NULL
	                HKEY	hkeyClass	NULL	NULL
	                DWORD	dwHotKey	0	0
	                union		{ hIcon = NULL, hMonitor = NULL }	{ hIcon = NULL, hMonitor = NULL }
	                HANDLE	hProcess	NULL	NULL
                */
                int ignore = 0;
                before(pExecInfo, ignore);
                if (ignore)
                    return ignore == 1;
                return ShellExecuteExW_real(pExecInfo);
            }
        public:
            static inline CallbackList<void(SHELLEXECUTEINFOW* pExecInfo, int& ignore)> before;

            EventShellExecute() {
                IbDetourAttach(&ShellExecuteExW_real, ShellExecuteExW_detour);
            }
            ~EventShellExecute() {
                IbDetourDetach(&ShellExecuteExW_real, ShellExecuteExW_detour);
            }
        };

        /*
        class VFile {
            static inline HANDLE fake_handle = Addr(CreateFileW).align_up(4) + 1;

            static inline decltype(&CreateFileW) CreateFileW_real = CreateFileW;
            static inline HANDLE WINAPI CreateFileW_detour(
                _In_ LPCWSTR lpFileName,
                _In_ DWORD dwDesiredAccess,
                _In_ DWORD dwShareMode,
                _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                _In_ DWORD dwCreationDisposition,
                _In_ DWORD dwFlagsAndAttributes,
                _In_opt_ HANDLE hTemplateFile
            )
            {
                if (wstring_view(lpFileName).rfind(LR"(V:\Ib\GetFolderSize\)", 0) == 0) {
                    return fake_handle;
                }
                return CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
            }
        public:
            VFile() {
                IbDetourAttach(&CreateFileW_real, CreateFileW_detour);
            }

            ~VFile() {
                IbDetourDetach(&CreateFileW_real, CreateFileW_detour);
            }
        };
        */
    }
}