#include <util.h>
#include <minwindef.h>
#include <processenv.h>

std::wstring getCurrentDirectory() {
    wchar_t buffer[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, buffer);
    return buffer;
}

