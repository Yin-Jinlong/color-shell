#pragma once

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <string>

#define MAX_LINE_LENGTH 1024

#define DLL_OUT __declspec(dllexport)
#define DLL_IN __declspec(dllimport)
#define USE_RET [[nodiscard]]

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;


namespace csh{
    class File;
    class FileStream;
}