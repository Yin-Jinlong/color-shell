#include <color-shell.h>
#include "str/wstring-util.h"
#include "inner-cmd.h"
#include <windows.h>

ColorShell::ColorShell() = default;

ColorShell::~ColorShell() = default;

void split(const std::wstring &line, _Out_ std::wstring &cmd, _Out_ std::wstring &arg) {
    std::wstring s = wstr_trim(line, true, false);
    if (s.empty())
        return;
    auto p = s.find_first_of(' ');
    if (p == std::string::npos) {
        cmd = s;
        return;
    }
    cmd = s.substr(0, p);
    arg = s.substr(p + 1);
}

bool ColorShell::run(std::wstring line,std::wstring &cmd, int &rc) {
    std::wstring arg;
    split(line, cmd, arg);
    if (cmd == L"exit")
        return true;
    if (cmd == L"cd") {
        rc = csh::cd(arg);
    } else {
        PROCESS_INFORMATION pi={};
        STARTUPINFOW si={};
        si.cb = sizeof(si);

        if (!CreateProcessW(
                nullptr,
                line.data(),
                nullptr,
                nullptr,
                true,
                0,
                nullptr,
                nullptr,
                &si,
                &pi
        )){
            throw std::runtime_error("CreateProcess failed");
        }
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &(DWORD&)rc);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    return false;
}

