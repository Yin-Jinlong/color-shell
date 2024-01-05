#pragma once

#include <predef.h>
#include <Windows.h>
#include <vector>

DLL_OUT std::wstring wstr_trim(const std::wstring &str, bool start = true, bool end = true);

constexpr inline i64 WSTR_HASH(wchar_t *str);

DLL_OUT std::wstring str_to_wstr(UINT codepage, const std::string &str);

DLL_OUT int wstr_split(const std::wstring &str, std::vector<std::wstring> &out, wchar_t delim = L' ');