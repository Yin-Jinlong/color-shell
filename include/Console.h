#pragma once

#include <predef.h>
#include "Color.h"

class DLL_OUT Console {
private:

    static bool withColor;
public:

    static void setColorMode(bool on);

    static void reset(bool all = false);

    static void setBackgroundColor(csh::Color &color) {
        setBackgroundColor(color.r, color.g, color.b);
    }

    static void setBackgroundColor(u8 r, u8 g, u8 b);

    static void setForegroundColor(csh::Color &color) {
        setForegroundColor(color.r, color.g, color.b);
    }

    static void setForegroundColor(u8 r, u8 g, u8 b);

    static void setColor(csh::Color &fc, csh::Color &bc) {
        setColor(fc.r, fc.g, fc.b, bc.r, bc.g, bc.b);
    }

    static void setColor(u8 fr, u8 fg, u8 fb, u8 br, u8 bg, u8 bb);

    static void moveCursorUp(int n);

    static void moveCursorDown(int n);

    static void moveCursorLeft(int n);

    static void moveCursorRight(int n);

    const static int CLEAR_CURSOR_TO_END   = 0;
    const static int CLEAR_CURSOR_TO_START = 1;
    const static int CLEAR_VIEW_ALL        = 2;
    const static int CLEAR_ALL             = 3;

    static void clear(int flag);

    static void print(const std::wstring &str);

    static void print(const wchar_t *str);

    static void print(wchar_t c);

    static void printNum(int i);


};


