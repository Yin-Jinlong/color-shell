#pragma once

#include <predef.h>

DLL_OUT bool is_high_surrogate(wchar_t wc);

DLL_OUT bool is_low_surrogate(wchar_t wc);

/**
 * 判断是否是全角字符
 * @param wc 字符
 * @return 是否是全角字符
 */
DLL_OUT bool is_full_width_char(wchar_t wc);
