#pragma once

#include <predef.h>
#include <Windows.h>

extern LIB_API std::wstring getCurrentDirectory();

LIB_API CONSOLE_SCREEN_BUFFER_INFOEX getConsoleBufferInfo();
