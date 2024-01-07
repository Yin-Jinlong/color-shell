#include <str/wstring-util.h>

bool isWhitespace(wchar_t c) {
    return c == L' ' || c == L'\t' || c == L'\r' || c == L'\n';
}

wstr wstr_trim(const wstr &str, bool start, bool end) {
    const wchar_t *r  = str.c_str();
    int           len = static_cast<int>(str.size());
    int           s   = 0, e = static_cast<int>(str.size());
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

wstr str_to_wstr(UINT codepage, const std::string &str) {
    if (str.empty())
        return L"";
    int  len  = MultiByteToWideChar(codepage, 0, str.c_str(), -1, nullptr, 0);
    auto *buf = (wchar_t *) malloc(sizeof(wchar_t) * len);
    MultiByteToWideChar(codepage, 0, str.c_str(), -1, buf, len);
    wstr r(buf);
    free(buf);
    return r;
}

int wstr_split(const wstr &str, std::vector<wstr> &out, wchar_t delim) {
    wstr     s = str;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == delim) {
            out.push_back(s.substr(0, i));
            s = s.substr(i + 1);
            i = -1;
        }
    }
    out.push_back(s);
    return static_cast<int>(out.size());
}
