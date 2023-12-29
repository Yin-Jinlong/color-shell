#pragma once

#include <str/wstring-util.h>

bool isWhitespace(wchar_t c) {
    return c == L' '|| c == L'\t' || c == L'\r' || c == L'\n';
}

std::wstring wstr_trim(const std::wstring& str, bool start, bool end) {
    auto r = str.c_str();
    auto len = (int) str.size();
    auto s = 0, e = (int) str.size();
    if (start) {
        while (len && isWhitespace(r[s])) {
            s++;
            len--;
        }
    }
    if (end) {
        while (len && isWhitespace(r[e-1])) {
            e--;
            len--;
        }
    }
    return str.substr(s, e - s + 1);
}