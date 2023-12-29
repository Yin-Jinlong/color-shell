#pragma once

#include <predef.h>

LIB_API std::wstring wstr_trim(const std::wstring& str,bool start= true,bool end= true);

constexpr inline i64 WSTR_HASH(wchar_t *str);
