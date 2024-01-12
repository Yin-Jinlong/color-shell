#include <str/string-util.h>
#include <tiny-unicode.h>

bool isWhitespace(wchar_t c) {
    return c == L' ' || c == L'\t' || c == L'\r' || c == L'\n';
}

str strTrim(const str &str, bool start, bool end) {
    const char *r  = str.c_str();
    int        len = static_cast<int>(str.size());
    int        s   = 0, e = static_cast<int>(str.size());
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


int strSplit(const str &string, std::vector<str> &out, char delim) {
    str      s = string;
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

str strGetExt(const str &path) {
    size_t p = path.find_last_of('.');
    if (p == str::npos)
        return "";
    str       ext = path.substr(p);
    for (auto &i: ext) {
        char c = i;
        if (c >= 'A' && c <= 'Z')
            c -= 'A' - 'a';

        i = c;
    }
    return ext;
}

str u32StrToStr(const u32str &s) {
    int  len  = static_cast<int>(s.size() * 4+1);
    if (!len)
        return "";
    auto *buf = new u8_char[len];
    tu_utf32_to_utf8(s.c_str(), static_cast<int>(s.size()), buf, &len);
    str r(buf);
    delete[] buf;
    return r;
}

u32str strToU32Str(const str &s) {
    int  len  = static_cast<int>(s.size())+1;
    if (!len)
        return {};
    auto *buf = new u32_char[len];
    tu_utf8_to_utf32(s.c_str(), static_cast<int>(s.size()), buf, &len);
    u32str r(buf);
    delete[] buf;
    return r;
}
