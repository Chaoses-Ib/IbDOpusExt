// C++20
#include "sigmatch.hpp"
#include <sigmatch/sigmatch.hpp>
#include <Psapi.h>
#include "helper.hpp"

ib::Addr sigmatch_RunCommandB4_True() {
    using namespace sigmatch_literals;

    MODULEINFO info;
    if (!GetModuleInformation(GetCurrentProcess(), ib::ModuleFactory::current_process().handle, &info, sizeof info))
        return nullptr;

    sigmatch::this_process_target target;
    sigmatch::search_context context = target.in_range({ info.lpBaseOfDll, info.SizeOfImage });

    auto search = [&target, &context](sigmatch::signature sig) -> ib::Addr {
        std::vector<const std::byte*> matches = context.search(sig).matches();
        if (matches.empty()) {
            if constexpr (debug)
                DebugOStream() << L"search: Found zero result\n";
            return nullptr;
        }
        if constexpr (debug)
            if (matches.size() > 1)
                DebugOStream() << L"search: Found more than one results\n";
        return (void*)matches[0];
    };
    auto search_entry = [&target, &context, &search](sigmatch::signature sig, sigmatch::signature entry_sig, size_t entry_range) -> ib::Addr {
        ib::Addr code = search(sig);
        if (!code)
            return nullptr;

        // not stable
        //return find_entry_CC(matches[0]);

        std::vector<const std::byte*> matches = target.in_range({ code - entry_range, entry_range }).search(entry_sig).matches();
        if (matches.empty())
            return nullptr;

        return (void*)matches.back();
    };
    auto search_call = [&target, &context, &search]<typename offset>(sigmatch::signature sig, size_t call_offset) -> ib::Addr {
        ib::Addr code = search(sig);
        if (!code)
            return nullptr;

        ib::Addr offset_addr = code + call_offset;
        ib::Addr eip = offset_addr + sizeof(offset);
        return eip + *(offset*)offset_addr;
    };

    ib::Addr RunCommandB4_True = search_call.template operator()<int32_t>("E8 ?? ?? ?? ?? 49 89 ?? ?? ?? ?? ?? 41 8B FD"_sig, 1);

    if constexpr (debug)
        DebugOStream() << "sigmatch_RunCommandB4_True: " << std::hex << (RunCommandB4_True - info.lpBaseOfDll) << L'\n';

    return RunCommandB4_True;
}

ib::Addr sigmatch_Prefs() {
    using namespace sigmatch_literals;

    MODULEINFO info;
    if (!GetModuleInformation(GetCurrentProcess(), ib::ModuleFactory::current_process().handle, &info, sizeof info))
        return nullptr;

    sigmatch::this_process_target target;
    sigmatch::search_context context = target.in_range({ info.lpBaseOfDll, info.SizeOfImage });

    auto search = [&target, &context](sigmatch::signature sig) -> ib::Addr {
        std::vector<const std::byte*> matches = context.search(sig).matches();
        if (matches.empty()) {
            if constexpr (debug)
                DebugOStream() << L"search: Found zero result\n";
            return nullptr;
        }
        if constexpr (debug)
            if (matches.size() > 1)
                DebugOStream() << L"search: Found more than one results\n";
        return (void*)matches[0];
    };
    auto search_entry = [&target, &context, &search](sigmatch::signature sig, sigmatch::signature entry_sig, size_t entry_range) -> ib::Addr {
        ib::Addr code = search(sig);
        if (!code)
            return nullptr;

        // not stable
        //return find_entry_CC(matches[0]);

        std::vector<const std::byte*> matches = target.in_range({ code - entry_range, entry_range }).search(entry_sig).matches();
        if (matches.empty())
            return nullptr;

        return (void*)matches.back();
    };
    auto search_call = [&target, &context, &search]<typename offset>(sigmatch::signature sig, size_t call_offset) -> ib::Addr {
        ib::Addr code = search(sig);
        if (!code)
            return nullptr;

        ib::Addr offset_addr = code + call_offset;
        ib::Addr eip = offset_addr + sizeof(offset);
        return eip + *(offset*)offset_addr;
    };

    ib::Addr Prefs = search_call.template operator() < int32_t > ("4C ?? ?? ?? ?? ?? ?? 49 8B ?? ?? ?? ?? ?? F6 80"_sig, 3);

    if constexpr (debug)
        DebugOStream() << "sigmatch_Prefs: " << std::hex << (Prefs - info.lpBaseOfDll) << L'\n';

    return Prefs;
}

size_t sigmatch_MaxThumbSize() {
    using namespace sigmatch_literals;

    MODULEINFO info;
    if (!GetModuleInformation(GetCurrentProcess(), ib::ModuleFactory::current_process().handle, &info, sizeof info))
        return 0;

    sigmatch::this_process_target target;
    sigmatch::search_context context = target.in_range({ info.lpBaseOfDll, info.SizeOfImage });

    auto search = [&target, &context](sigmatch::signature sig) -> ib::Addr {
        std::vector<const std::byte*> matches = context.search(sig).matches();
        if (matches.empty()) {
            if constexpr (debug)
                DebugOStream() << L"search: Found zero result\n";
            return nullptr;
        }
        if constexpr (debug)
            if (matches.size() > 1)
                DebugOStream() << L"search: Found more than one results\n";
        return (void*)matches[0];
    };
    auto search_entry = [&target, &context, &search](sigmatch::signature sig, sigmatch::signature entry_sig, size_t entry_range) -> ib::Addr {
        ib::Addr code = search(sig);
        if (!code)
            return nullptr;

        // not stable
        //return find_entry_CC(matches[0]);

        std::vector<const std::byte*> matches = target.in_range({ code - entry_range, entry_range }).search(entry_sig).matches();
        if (matches.empty())
            return nullptr;

        return (void*)matches.back();
    };
    auto search_call = [&target, &context, &search]<typename offset>(sigmatch::signature sig, size_t call_offset) -> ib::Addr {
        ib::Addr code = search(sig);
        if (!code)
            return nullptr;

        ib::Addr offset_addr = code + call_offset;
        ib::Addr eip = offset_addr + sizeof(offset);
        return eip + *(offset*)offset_addr;
    };

    int32_t offset = *(uint32_t*)(search("48 ?? ?? ?? ?? ?? ?? 8B 88 ?? ?? ?? ?? 41 89 ?? ?? 48 ?? ?? ?? ?? ?? ?? 8B 88 ?? ?? ?? ?? 41 89"_sig) + 7 + 2);

    if constexpr (debug)
        DebugOStream() << "sigmatch_MaxThumbSize: " << std::hex << offset << L'\n';

    return offset;
}