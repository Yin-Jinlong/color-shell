#include "Console.h"

bool                       Console::withColor = true;
CONSOLE_SCREEN_BUFFER_INFO Console::csbi      = {};


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
    printf(L"\033[48;2;{};{};{}m",
           r, g, b);
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
    printf(L"\033[38;2;{};{};{};48;2;{};{};{}m",
           fr, fg, fb,
           br, bg, bb);
}

void Console::moveCursorUp(int n) {
    if (n < 1)
        return;
    printf(L"\033[{}A", n);
}

void Console::moveCursorDown(int n) {
    if (n < 1)
        return;
    printf(L"\033[{}B", n);
}

void Console::moveCursorLeft(int n) {
    if (n < 1)
        return;
    printf(L"\033[{}D", n);
}

void Console::moveCursorRight(int n) {
    if (n < 1)
        return;
    printf(L"\033[{}C", n);
}

void Console::clear(int flag) {
    printf(L"\033[{}J", flag);
}

void Console::print(const wstr &str) {
    std::wcout << str;
}

void Console::println(const wstr &str) {
    print(str);
    println();
}

void Console::print(const wchar_t *str) {
    if (!str)
        return;
    std::wcout << str;
}

void Console::println(const wchar_t *str) {
    print(str);
    println();
}

void Console::print(wchar_t c) {
    std::wcout << c;
}

void Console::println(wchar_t c) {
    print(c);
    println();
}

void Console::println() {
    print('\n');
}

void update(CONSOLE_SCREEN_BUFFER_INFO &csbi) {
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
}

void Console::getCursorPosition(COORD &coord) {
    update(csbi);
    coord = csbi.dwCursorPosition;
}

void Console::getCursorPosition(int &x, int &y) {
    update(csbi);
    x = csbi.dwCursorPosition.X;
    y = csbi.dwCursorPosition.Y;
}

void Console::setCursorPosition(int x, int y) {
    printf(L"\033[{};{}H", y + 1, x + 1);
}

void Console::setCursorPosition(const COORD &coord) {
    setCursorPosition(coord.X, coord.Y);
}

void Console::getBufferSize(COORD &size) {
    update(csbi);
    size = csbi.dwSize;
}

void Console::save() {
    print(L"\033[s");
}

void Console::restore() {
    print(L"\033[u");
}
