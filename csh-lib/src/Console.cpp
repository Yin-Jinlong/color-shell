#include <windows.h>
#include "Console.h"


bool Console::withColor = true;


void Console::setColorMode(bool on) {
    withColor = on;
}


void Console::reset(bool all) {
    if (all)
        std::wcout << L"\033c";
    else
        std::wcout << L"\033[0m";
}

void Console::setBackgroundColor(u8 r, u8 g, u8 b) {
    if (!withColor)
        return;
    std::wstring s = L"\033[48;2;";
    s += std::to_wstring(r);
    s += L";";
    s += std::to_wstring(g);
    s += L";";
    s += std::to_wstring(b);
    s += L"m";
    std::wcout << s;
}

void Console::setForegroundColor(u8 r, u8 g, u8 b) {
    if (!withColor)
        return;
    std::wstring s = L"\033[38;2;";
    s += std::to_wstring(r);
    s += L";";
    s += std::to_wstring(g);
    s += L";";
    s += std::to_wstring(b);
    s += L"m";
    std::wcout << s;
}

void Console::setColor(u8 fr, u8 fg, u8 fb, u8 br, u8 bg, u8 bb) {
    if (!withColor)
        return;
    std::wstring s = L"\033[38;2;";
    s += std::to_wstring(fr);
    s += L";";
    s += std::to_wstring(fg);
    s += L";";
    s += std::to_wstring(fb);
    s += L";48;2;";
    s += std::to_wstring(br);
    s += L";";
    s += std::to_wstring(bg);
    s += L";";
    s += std::to_wstring(bb);
    s += L"m";
    std::wcout << s;
}

void Console::print(const std::wstring &str) {
    print(str.c_str());
}

void Console::print(const wchar_t *str) {
    if (!str)
        return;
    char buf[MAX_LINE_LENGTH];
    if (!WideCharToMultiByte(
            CP_UTF8, 0,
            str, -1,
            buf, MAX_LINE_LENGTH,
            nullptr, nullptr)) {
        throw std::runtime_error("Error : " + std::to_string(GetLastError()));
    }
    std::cout << buf;
}

void Console::print(const std::string &str) {
    print(str.c_str());
}

void Console::print(const char *str) {
    std::cout << str;
}
