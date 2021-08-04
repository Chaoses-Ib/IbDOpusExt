#pragma once

#ifdef _MANAGED
#include <msclr\gcroot.h>
#define GCROOT(T) msclr::gcroot<T>
#else
#define GCROOT(T) void*
#endif

#include <string>
#include <Windows.h>

namespace gui {
#pragma managed(push, off)
    struct ConfigData {
        struct FileOperationsData {
            struct LoggingData {
                bool MaxUndoNum_enable;
                unsigned char MaxUndoNum;
            } Logging;
        } FileOperations;

        struct FoldersData {
            struct FolderBehaviourData {
                bool GetSizesByEverything;
            } FolderBehaviour;
        } Folders;
    };
#pragma managed(pop)

    class Config {
    public:
        static void Init(std::wstring_view filename);
        static ConfigData GetConfig();
        static inline void (*ApplyCallback)(ConfigData&&);
    };

    class PreferenceShow {
        GCROOT(GuiShell::PreferenceShow^) preferences;
    public:
        PreferenceShow();
        ~PreferenceShow();
        bool Show();
    };
}