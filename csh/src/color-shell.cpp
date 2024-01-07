#include <color-shell.h>
#include "str/wstring-util.h"
#include "inner-cmd.h"
#include "util.h"
#include "File.h"
#include <Windows.h>
#include <algorithm>

extern csh::CmdHistory history;

ColorShell::ColorShell() {
    wstr    path = getEnv(L"PATH");
    wchar_t buf[MAX_PATH];
    GetModuleFileNameW(nullptr, buf, MAX_PATH);
    wstr mp = buf;
    while (mp[mp.size() - 1] != '\\')
        mp.pop_back();
    // 当前工作目录优先级最高
    paths.push_back(getCurrentDirectory());
    // 其次是程序目录
    paths.push_back(mp);
    // 最后是PATH
    wstr_split(path, paths, ';');
}

ColorShell::~ColorShell() = default;

void split(const wstr &line, _Out_ wstr &cmd, _Out_ wstr &arg) {
    wstr s = wstr_trim(line, true, false);
    if (s.empty())
        return;
    size_t p = s.find_first_of(' ');
    if (p == std::string::npos) {
        cmd = s;
        return;
    }
    cmd      = s.substr(0, p);
    arg      = s.substr(p + 1);
}

wstr getExt(const wstr &path) {
    size_t p = path.find_last_of('.');
    if (p == wstr::npos)
        return L"";
    wstr         ext = path.substr(p);
    for (wchar_t &i: ext) {
        wchar_t c = i;
        if (c >= 'A' && c <= 'Z')
            c -= 'A' - 'a';

        i = c;
    }
    return ext;
}

#define EXT_I_CMD 1
#define EXT_I_BAT 2
#define EXT_I_PS1 3

static std::vector<wstr> exts = {L".exe", L".cmd", L".bat", L".ps1"};

wstr checkExt(
        const std::vector<wstr> &paths,
        const wstr &cmd
) {
    bool noExt = true;
    wstr ext   = getExt(wstr_trim(cmd, true, false));

    for (const wstr &e: exts) {
        if (e == ext) {
            noExt = false;
            break;
        }
    }

    for (const wstr &p: paths) {
        if (!noExt) {
            const csh::File &file = csh::File(p, cmd);
            if (file.exists()) {
                if (ext == exts[0]) {
                    return file.getPath();
                }
                return ext;
            }
            continue;
        }
        // 补全后缀找
        for (const wstr &e: exts) {
            const csh::File &file = csh::File(p, cmd + e);
            if (file.exists()) {
                if (e == exts[0]) {
                    return file.getPath();
                }
                return e;
            }
        }
    }
    return L"";
}

void runCmd(wstr cmdLine, DWORD &rc, const wchar_t *app = nullptr) {
    PROCESS_INFORMATION pi = {};
    STARTUPINFOW        si = {};
    si.cb = sizeof(si);

    if (!CreateProcessW(
            app,
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

bool ColorShell::run(wstr line, wstr &cmd, int &rc, wstr &err) {
    err.clear();
    wstr arg;
    split(line, cmd, arg);
    if (cmd == L"exit") {
        return false;
    } else if (cmd == L"cd") {
        rc = csh::cd(arg);
        if (!rc) { // 目录切换，更新路径
            paths[0] = getCurrentDirectory();
        }
        return true;
    } else if (cmd == L"history") {
        rc = csh::history(history, arg);
        return true;
    }

    wstr t = checkExt(paths, cmd);
    if (t.empty()) {
        err = std::format(L"Unknown command or executable or runnable script file : '{}'", cmd);
    } else if (t == exts[EXT_I_CMD] || t == exts[EXT_I_BAT]) {
        runCmd(std::format(L"cmd /c {}", line), (DWORD &) rc);
    } else if (t == exts[EXT_I_PS1]) {
        runCmd(std::format(L"powershell /c {}", line), (DWORD &) rc);
    } else {
        runCmd(line, (DWORD &) rc, t.c_str());
    }
    return true;
}

#undef EXTI_CMD
#undef EXTI_BAT
#undef EXTI_PS1
