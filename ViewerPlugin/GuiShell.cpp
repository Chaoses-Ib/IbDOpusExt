#include "GuiShell.hpp"
#include <msclr\marshal_cppstd.h>

using namespace System;
using namespace msclr::interop;

constexpr bool debug =
#ifdef _DEBUG
    1;
#else
    0;
#endif

namespace gui {
    ConfigData ConvertConfigData(GuiShell::ConfigData^ config_) {
        ConfigData config;

        if (config.FileOperations.Logging.MaxUndoNum_enable = config_->FileOperations->Logging->MaxUndoNum != 10) {
            config.FileOperations.Logging.MaxUndoNum = config_->FileOperations->Logging->MaxUndoNum;
        }

        config.Folders.FolderBehaviour.GetSizesByEverything = config_->Folders->FolderBehaviour->GetSizesByEverything;

        return config;
    }

    void ApplyAction(GuiShell::ConfigData^ config) {
        Config::ApplyCallback(ConvertConfigData(config));
    }

    void Config::Init(std::wstring_view filename) {
        GuiShell::Config::SetUnhandledExceptionHandler();
        GuiShell::Config::ApplyAction += gcnew Action<GuiShell::ConfigData^>(ApplyAction);
        GuiShell::Config::Filename = marshal_as<String^>(std::wstring(filename));
    }

    ConfigData Config::GetConfig() {
        return ConvertConfigData(GuiShell::Config::Read());
    }


    PreferenceShow::PreferenceShow() {
        if constexpr (debug)
            OutputDebugStringW(L"gui::PreferenceShow\n");
        preferences = gcnew GuiShell::PreferenceShow();
    }

    PreferenceShow::~PreferenceShow() {
        if constexpr (debug)
            OutputDebugStringW(L"gui::~PreferenceShow\n");
    }

    bool PreferenceShow::Show() {
        return preferences->Show();
    }
}