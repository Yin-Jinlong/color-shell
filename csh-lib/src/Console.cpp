#include "Console.h"
#include "str/string-util.h"

bool                       Console::withColor = true;
CONSOLE_SCREEN_BUFFER_INFO Console::csbi      = {};


void Console::setColorMode(bool on) {
    withColor = on;
}

void Console::reset(bool all) {
    if (all)
        print("\033c");
    else
        print("\033[0m");
}

void Console::setBackgroundColor(u8 r, u8 g, u8 b) {
    if (!withColor)
        return;
    printf("\033[48;2;{};{};{}m",
           csh::Arg(r), csh::Arg(g), csh::Arg(b));
}

void Console::setForegroundColor(u8 r, u8 g, u8 b) {
    if (!withColor)
        return;
    printf("\033[38;2;{};{};{}m",
           r, g, b);
}

void Console::setColor(u8 fr, u8 fg, u8 fb, u8 br, u8 bg, u8 bb) {
    if (!withColor)
        return;
    printf("\033[38;2;{};{};{};48;2;{};{};{}m",
           fr, fg, fb,
           br, bg, bb);
}

void Console::moveCursorUp(int n) {
    if (n < 1)
        return;
    printf("\033[{}A", n);
}

void Console::moveCursorDown(int n) {
    if (n < 1)
        return;
    printf("\033[{}B", n);
}

void Console::moveCursorLeft(int n, bool autoUp) {
    if (n < 1)
        return;
    if (!autoUp) {
        printf("\033[{}D", n);
        return;
    }
    COORD np, size;
    getCursorPosition(np);
    getBufferSize(size);

    if (np.X - n < 0) {
        printf("\033[A\033[{}C", size.X);
        n -= np.X + 1;
    }
    while (n > size.X) {
        printf("\033[A\033[{}C", size.X);
        n -= size.X;
    }
    if (n > 0)
        printf("\033[{}D", n);
}

void Console::moveCursorRight(int n, bool autoDown) {
    if (n < 1)
        return;
    if (!autoDown) {
        printf("\033[{}C", n);
        return;
    }
    COORD np, size;
    getCursorPosition(np);
    getBufferSize(size);

    if (np.X + n >= size.X) {
        print("\r\n");
        n -= size.X - np.X;
    }
    while (n > size.X) {
        print("\r\n");
        n -= size.X;
    }
    if (n > 0)
        printf("\033[{}C", n);
}

void Console::clear(int flag) {
    printf("\033[{}J", flag);
}

void Console::print(const u32str &s) {
    print(u32StrToStr(s));
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
    printf("\033[{};{}H", y + 1, x + 1);
}

void Console::setCursorPosition(const COORD &coord) {
    setCursorPosition(coord.X, coord.Y);
}

void Console::getBufferSize(COORD &size) {
    update(csbi);
    size = csbi.dwSize;
}

void Console::save() {
    print("\033[s");
}

void Console::restore() {
    print("\033[u");
}

