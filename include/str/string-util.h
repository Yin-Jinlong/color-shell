#pragma once

#include <predef.h>
#include <Windows.h>

/**
 * 字符串去首尾空格
 * @param str 字符串
 * @param start 去除起始空格
 * @param end 去除结束空格
 * @return 去除空格后的字符串
 */
DLL_OUT str strTrim(const str &str, bool start = true, bool end = true);

/**
 * 字符串哈希
 * @param str 字符串
 * @return 哈希值
 */
constexpr inline i64 WSTR_HASH(wchar_t *str);

/**
 * 分割字符串
 * @param string 字符串
 * @param out 输出
 * @param delim 分隔符
 * @return 数量
 */
DLL_OUT int strSplit(const str &string, std::vector<str> &out, char delim = L' ');

/**
 * 获取路径扩展名
 * @param path 路径
 * @return 扩展名(带.)或空
 */
DLL_OUT str strGetExt(const str &path);
