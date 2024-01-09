#include <str/wstring-util.h>

bool isWhitespace(wchar_t c) {
    return c == L' ' || c == L'\t' || c == L'\r' || c == L'\n';
}

wstr wstrTrim(const wstr &str, bool start, bool end) {
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
    return str.substr(s, e - s);
}

constexpr inline i64 WSTR_HASH(wchar_t *str) {
    i64 hc = 0;
    while (*str) {
        hc = hc * 31 + *str++;
    }
    return hc;
}

wstr strToWstr(UINT codepage, const std::string &str) {
    if (str.empty())
        return L"";
    int  len  = MultiByteToWideChar(codepage, 0, str.c_str(), -1, nullptr, 0);
    auto *buf = (wchar_t *) malloc(sizeof(wchar_t) * len);
    MultiByteToWideChar(codepage, 0, str.c_str(), -1, buf, len);
    wstr r(buf);
    free(buf);
    return r;
}

int wstrSplit(const wstr &str, std::vector<wstr> &out, wchar_t delim) {
    wstr     s = str;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == delim) {
            out.push_back(s.substr(0, i));
            s = s.substr(i + 1);
            i = -1;
            while (!s.empty() && s[0] == delim) {
                s.erase(0, 1);
            }
        }
    }
    out.push_back(s);
    return static_cast<int>(out.size());
}

wstr wstrGetExt(const wstr &path){
    size_t p = path.find_last_of('.');
    if (p == wstr::npos)
        return L"";
    wstr         ext = path.substr(p);
    for (wchar_t &i: ext) {
        wchar_t c = i;
        if (c >= 'A' && c <= 'Z')
            c -= 'A' - 'a';

        i = c;
    }
    return ext;
}
