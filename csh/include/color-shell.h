#pragma once

#include <predef.h>
#include <vector>

class ColorShell {
private:
    std::vector<std::wstring> paths;
public:
    ColorShell();

    ~ColorShell();

    bool run(std::wstring line, std::wstring &cmd, int &rc);
};