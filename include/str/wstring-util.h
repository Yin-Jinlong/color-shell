#pragma once

#include <predef.h>
#include <Windows.h>

DLL_OUT std::wstring wstr_trim(const std::wstring &str, bool start = true, bool end = true);

constexpr inline i64 WSTR_HASH(wchar_t *str);

DLL_OUT std::wstring str_to_wstr(UINT codepage, const std::string &str);
