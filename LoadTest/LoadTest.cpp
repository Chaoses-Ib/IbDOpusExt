#include <Windows.h>

#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <fmt/locale.h>

using namespace std;

int wmain(int argc, wchar_t* argv[])
{
    _setmode(_fileno(stdout), _O_WTEXT);

    if (argc < 2) {
        fmt::print(L"Argument please.\n");
        system("pause");
        return 0;
    }

    system("pause");
    
    const wchar_t* filename = argv[1];
    fmt::print(L"{}\n", filename);

    HMODULE h = LoadLibraryExW(filename, 0, LOAD_WITH_ALTERED_SEARCH_PATH);
    DWORD error = GetLastError();
    fmt::print(L"{}, {}\n", (void*)h, error);

    if (h) {
        FARPROC p = GetProcAddress(h, "DVP_InitEx");
        error = GetLastError();
        fmt::print(L"DVP_InitEx: {}, {}\n", (void*)p, error);
    }

    system("pause");
}