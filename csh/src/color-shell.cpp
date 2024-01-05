#include <color-shell.h>
#include "str/wstring-util.h"
#include "inner-cmd.h"
#include "util.h"
#include "File.h"
#include <Windows.h>
#include <algorithm>

ColorShell::ColorShell() {
    auto path = getEnv(L"PATH");
    wstr_split(path, paths, ';');
}

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

bool checkCmdScript(const std::vector<std::wstring> &paths, const std::wstring &cmd) {
    auto f = wstr_trim(cmd, true, false);
    auto noExt = true;
    if (f.ends_with(L"bat") || f.ends_with(L"cmd")) {
        noExt = false;
    }
    return std::any_of(paths.begin(), paths.end(), [&](const std::wstring &p) {
        if (noExt) {
            csh::File fileBat(p, cmd + L".bat");
            csh::File fileCmd(p, cmd + L".cmd");
            return fileBat.exists() || fileCmd.exists();
        }
        csh::File file(p, cmd);
        return file.exists();
    });
}

bool checkPsScript(const std::vector<std::wstring> &paths, const std::wstring &cmd) {
    auto f = wstr_trim(cmd, true, false);
    auto noExt = true;
    if (f.ends_with(L"ps1")) {
        noExt = false;
    }
    return std::any_of(paths.begin(), paths.end(), [&](const std::wstring &p) {
        if (noExt) {
            csh::File filePs1(p, cmd + L".ps1");
            return filePs1.exists();
        }
        csh::File file(p, cmd);
        return file.exists();
    });
}

void runCmd(std::wstring cmdLine, DWORD &rc) {
    PROCESS_INFORMATION pi = {};
    STARTUPINFOW si = {};
    si.cb = sizeof(si);

    if (!CreateProcessW(
            nullptr,
            cmdLine.data(),
            nullptr,
            nullptr,
            true,
            0,
            nullptr,
            nullptr,
            &si,
            &pi
    )) {
        throw std::runtime_error("CreateProcess failed");
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    GetExitCodeProcess(pi.hProcess, &(DWORD &) rc);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

bool ColorShell::run(std::wstring line, std::wstring &cmd, int &rc) {
    std::wstring arg;
    split(line, cmd, arg);
    if (cmd == L"exit")
        return true;
    if (cmd == L"cd") {
        rc = csh::cd(arg);
    } else if (checkCmdScript(paths, cmd)) {
        runCmd(std::format(L"cmd /c {}", line), (DWORD &) rc);
    } else if (checkPsScript(paths, cmd)) {
        runCmd(std::format(L"powershell /c {}", line), (DWORD &) rc);
    } else {
        runCmd(line, (DWORD &) rc);
    }
    return false;
}

