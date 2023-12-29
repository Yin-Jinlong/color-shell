#pragma once

#include <predef.h>

class ColorShell {
public:
    ColorShell();
    ~ColorShell();
    bool run(std::wstring line,std::wstring &cmd,int &rc);
};