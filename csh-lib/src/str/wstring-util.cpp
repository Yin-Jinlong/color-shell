#pragma once

#include <str/wstring-util.h>

bool isWhitespace(wchar_t c) {
    return c == L' ' || c == L'\t' || c == L'\r' || c == L'\n';
}

std::wstring wstr_trim(const std::wstring &str, bool start, bool end) {
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
        while (len && isWhitespace(r[e - 1])) {
            e--;
            len--;
        }
    }
    return str.substr(s, e - s + 1);
}

constexpr inline i64 WSTR_HASH(wchar_t *str) {
    i64 hc = 0;
    while (*str) {
        hc = hc * 31 + *str++;
    }
    return hc;
}

std::wstring str_to_wstr(UINT codepage, const std::string &str) {
    if (str.empty())
        return L"";
    auto len = MultiByteToWideChar(codepage, 0, str.c_str(), -1, nullptr, 0);
    auto *buf = (wchar_t *) malloc(sizeof(wchar_t) * len);
    MultiByteToWideChar(codepage, 0, str.c_str(), -1, buf, len);
    auto r = std::wstring((const wchar_t *) buf);
    free(buf);
    return r;
}

int wstr_split(const std::wstring &str, std::vector<std::wstring> &out, wchar_t delim) {
    auto s = str;
    for (auto i = 0; i < s.size(); i++) {
        if (s[i] == delim) {
            out.push_back(s.substr(0, i));
            s = s.substr(i + 1);
            i = -1;
        }
    }
    out.push_back(s);
    return out.size();
}