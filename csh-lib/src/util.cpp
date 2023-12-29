#include <util.h>
#include <minwindef.h>
#include <processenv.h>

std::wstring getCurrentDirectory() {
    wchar_t buffer[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, buffer);
    return buffer;
}

CONSOLE_SCREEN_BUFFER_INFOEX getConsoleBufferInfo() {
    CONSOLE_SCREEN_BUFFER_INFOEX info;
    GetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return info;
}