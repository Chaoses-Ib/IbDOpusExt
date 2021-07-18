#pragma once
#include "pch.h"
#include <utility>
#include "helper.hpp"
#include <eventpp/callbacklist.h>
#include <IbWinCppLib/WinCppLib.hpp>

namespace DOpus {
    using eventpp::CallbackList;

    namespace Modules {
        class dopus : public Module {
        public:
            dopus() : Module(*ModuleFactory::CurrentProcess()) {}
        };
    }

    class Prefs {
    public:
        Addr base;
        Prefs(Modules::dopus& dopus) : base( *((void**)dopus.base.offset(0x12BD9C0)) ) { }
    };

    namespace Thumbnails {
        class MaxSize {
            uint32_t* Pref_MaxThumbSize;
        public:
            MaxSize(Prefs& prefs) {
                Pref_MaxThumbSize = prefs.base.offset(0x1488);
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
        class EventExecuteCommands {
            //#TODO: iterator
            struct CommandLink
            {
                CommandLink* Next;
                CommandLink* Last;
                CommandLink* FinalLine;
                QWORD qword18_Num;
                WCHAR* Command;
            };
            
            static uint64_t RunCommandB4_TrueDetour(__int64 a1, CommandLink* commands, __int64 a3, __int64 a4, DWORD* a5) {
                CommandLink* cmd = commands;
                for (; cmd->Next; cmd = cmd->Next);
                cmd->Last->Next = nullptr;
                callbacks(reinterpret_cast<Command*>(commands->Next), reinterpret_cast<uint64_t>(commands->Command));
                cmd->Last->Next = cmd;
                return RunCommandB4_True(a1, commands, a3, a4, a5);
            }
            using RunCommandB4_True_t = decltype(RunCommandB4_TrueDetour);
            static inline RunCommandB4_True_t* RunCommandB4_True;
        public:
            struct Command {
                Command* Next;
                Command* Last;
                uint64_t : 64;
                uint64_t : 64;
                wchar* Command;  //may be nullptr
            };
            static inline CallbackList<void(Command* cmd, uint64_t n)> callbacks;
            EventExecuteCommands(Modules::dopus& dopus) {
                RunCommandB4_True = dopus.base.offset(0x49E9E0);
                IbDetourAttach(&RunCommandB4_True, RunCommandB4_TrueDetour);
            }
            ~EventExecuteCommands() {
                IbDetourDetach(&RunCommandB4_True, RunCommandB4_TrueDetour);
            }
        };
    }

    class Mem {
    public:
        static inline void* (*Alloc)(size_t size);
        static inline void* (*Free)(void* block);
        Mem(Modules::dopus& dopus) {
            Alloc = dopus.base.offset(0xE41630);
            Free = dopus.base.offset(0xE89A64);
        }
    };
}