#pragma once
#include <string>
#include <optional>
#include <functional>
#include "Detours/detours.h"
#include <xmmintrin.h>

using namespace std;
using wchar = wchar_t;
using byte_t = uint8_t; //since there is std::byte
using QWORD = int64_t;

void DebugOutput(const wchar* str);
void DebugOutput(const wstring &str);
void DebugOutput(const wstringstream &str);

template<typename T>
LONG IbDetourAttach(_Inout_ T* ppPointer, _In_ T pDetour) {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach((void**)ppPointer, pDetour);
    return DetourTransactionCommit();
}

template<typename T>
LONG IbDetourDetach(_Inout_ T* ppPointer, _In_ T pDetour) {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach((void**)ppPointer, pDetour);
    return DetourTransactionCommit();
}

using offset_t = int32_t; //since size_t can't be negative

struct addr {
    void* p;

    addr(void* p) : p(p) { }
    addr(byte_t* p) : p(p) { }

    operator void* () {
        return p;
    }
    operator byte_t* () {
        return (byte_t*)p;
    }
    template<typename T>
    operator T* () {
        return (T*)p;
    }
#ifdef _WIN64
    explicit operator uint64_t(){
        return (uint64_t)p;
    }
#else
    explicit operator uint32_t() {
        return (uint32_t)p;
    }
#endif

    addr offset(offset_t offset) {
        return addr((byte_t*)p + offset);
    }
    addr operator+(offset_t offset) {
        return this->offset(offset);
    }
    addr operator-(offset_t offset) {
        return this->offset(-offset);
    }

    bool Unprotected(size_t size, function<bool(addr)> f) {
        DWORD old_protect;
        if (!VirtualProtect(p, size, PAGE_EXECUTE_READWRITE, &old_protect)) //#TODO
            return false;
        bool success = f(*this);
        VirtualProtect(p, size, old_protect, &old_protect); //lpflOldProtect can't be nullptr
        return success;
    }
};

struct Module {
    union {
        HMODULE handle;
        addr base;
    };

    Module(HMODULE handle) : handle(handle) {};

    optional<wstring> GetPath() {
        wchar_t pathbuf[MAX_PATH];
        if (GetModuleFileNameW(handle, pathbuf, MAX_PATH))
            return wstring(pathbuf);
        else
            return nullopt;
    }

    bool Free() {
        return FreeLibrary(handle);
    }
};

struct makeModule {
    //doesn't increase reference count
    static optional<Module> Find(const wstring &module_name) {
        HMODULE h;
        if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, module_name.c_str(), &h))
            return Module(h);
        else
            return {};
    }

    static optional<Module> Load(const wstring &module_name) {
        auto h = LoadLibraryW(module_name.c_str());
        return h ? optional(Module(h)) : nullopt;
    }

    static optional<Module> CurrentProcess() {
        HMODULE h;
        if (GetModuleHandleExW(0, nullptr, &h))
            return Module(h);
        else
            return {};
    }
};