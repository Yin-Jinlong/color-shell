#pragma once

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <string>
#include <format>
#include <vector>
#include <array>
#include <map>

#define MAX_LINE_LENGTH 1024

#define DLL_OUT __declspec(dllexport)
#define DLL_IN __declspec(dllimport)
#define USE_RET [[nodiscard]]

#define MAYBE_UNUSED [[maybe_unused]]
#define DLL_OUTER_CALL MAYBE_UNUSED DLL_OUT

#define ARG_OUT
#define ARG_IN
#define ARG_INOUT

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef std::string    str;
typedef std::u8string  u8str;
typedef std::u32string u32str;
