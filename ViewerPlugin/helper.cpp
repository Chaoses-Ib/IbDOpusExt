#include "pch.h"
#include <sstream>
#include "helper.hpp"

void DebugOutputRaw(const wstring &str) {
#ifdef _DEBUG
    OutputDebugStringW(str.c_str());
#endif // _DEBUG
}

void DebugOutput(const wchar* str) {
    DebugOutputRaw(L"Ib: "s + str);
}

void DebugOutput(const wstring &str) {
    DebugOutputRaw(L"Ib: "s + str);
}

void DebugOutput(const wstringstream &str) {
    DebugOutputRaw(L"Ib: "s + str.str());
}