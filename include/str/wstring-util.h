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
DLL_OUT wstr wstrTrim(const wstr &str, bool start = true, bool end = true);

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
DLL_OUT wstr strToWstr(UINT codepage, const std::string &str);

/**
 * 分割字符串
 * @param str 字符串
 * @param out 输出
 * @param delim 分隔符
 * @return 数量
 */
DLL_OUT int wstrSplit(const wstr &str, std::vector<wstr> &out, wchar_t delim = L' ');

/**
 * 获取路径扩展名
 * @param path 路径
 * @return 扩展名(带.)或空
 */
DLL_OUT wstr wstrGetExt(const wstr &path);
