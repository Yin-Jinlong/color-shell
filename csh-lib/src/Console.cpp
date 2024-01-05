#include <windows.h>
#include "Console.h"


bool Console::withColor = true;


void Console::setColorMode(bool on) {
    withColor = on;
}


void Console::reset(bool all) {
    if (all)
        print(L"\033c");
    else
        print(L"\033[0m");
}

void Console::setBackgroundColor(u8 r, u8 g, u8 b) {
    if (!withColor)
        return;
    print(std::format(
            L"\033[48;2;{};{};{}m",
            r, g, b));
}

void Console::setForegroundColor(u8 r, u8 g, u8 b) {
    if (!withColor)
        return;
    print(std::format(
            L"\033[38;2;{};{};{}m",
            r, g, b));
}

void Console::setColor(u8 fr, u8 fg, u8 fb, u8 br, u8 bg, u8 bb) {
    if (!withColor)
        return;
    print(std::format(
            L"\033[38;2;{};{};{};48;2;{};{};{}m",
            fr, fg, fb,
            br, bg, bb));
}

void Console::moveCursorUp(int n) {
    print(std::format(L"\033[{}A", n));
}

void Console::moveCursorDown(int n) {
    print(std::format(L"\033[{}B", n));
}

void Console::moveCursorLeft(int n) {
    print(std::format(L"\033[{}D", n));
}

void Console::moveCursorRight(int n) {
    print(std::format(L"\033[{}C", n));
}

void Console::clear(int flag) {
    print(std::format(L"\033[{}J", flag));
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

void Console::print(wchar_t c) {
    std::wcout << c;
}

void Console::printNum(int i) {
    std::wcout << i;
}


