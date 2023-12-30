#pragma once

#include <predef.h>
#include <Windows.h>

extern DLL_OUT std::wstring getCurrentDirectory();

extern DLL_OUT std::string getProcessOutput(const std::wstring &cmdLine);
