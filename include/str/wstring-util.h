#pragma once

#include <predef.h>
#include <Windows.h>
#include <vector>

/**
 * 字符串去首尾空格
 * @param str 字符串
 * @param start 去除起始空格
 * @param end 去除结束空格
 * @return 去除空格后的字符串
 */
DLL_OUT std::wstring wstr_trim(const std::wstring &str, bool start = true, bool end = true);

/**
 * 字符串哈希
 * @param str 字符串
 * @return 哈希值
 */
constexpr inline i64 WSTR_HASH(wchar_t *str);

/**
 * 字符串转wstring
 * @param codepage 编码
 * @param str 字符串
 * @return wstring
 */
DLL_OUT std::wstring str_to_wstr(UINT codepage, const std::string &str);

/**
 * 分割字符串
 * @param str 字符串
 * @param out 输出
 * @param delim 分隔符
 * @return 数量
 */
DLL_OUT int wstr_split(const std::wstring &str, std::vector<std::wstring> &out, wchar_t delim = L' ');
