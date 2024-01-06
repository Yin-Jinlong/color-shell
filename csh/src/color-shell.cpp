#include <color-shell.h>
#include "str/wstring-util.h"
#include "inner-cmd.h"
#include "util.h"
#include "File.h"
#include <Windows.h>
#include <algorithm>

ColorShell::ColorShell() {
    auto    path = getEnv(L"PATH");
    wchar_t buf[MAX_PATH];
    GetModuleFileNameW(nullptr, buf, MAX_PATH);
    std::wstring mp = buf;
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

void split(const std::wstring &line, _Out_ std::wstring &cmd, _Out_ std::wstring &arg) {
    std::wstring s = wstr_trim(line, true, false);
    if (s.empty())
        return;
    auto p = s.find_first_of(' ');
    if (p == std::string::npos) {
        cmd = s;
        return;
    }
    cmd    = s.substr(0, p);
    arg    = s.substr(p + 1);
}

std::wstring get_ext(const std::wstring &path) {
    auto p = path.find_last_of('.');
    if (p == std::wstring::npos)
        return L"";
    auto     ext = path.substr(p);
    for (int i   = 0; i < ext.length(); ++i) {
        auto c = ext[i];
        if (c >= 'A' && c <= 'Z')
            c -= 'A' - 'a';
        ext[i] = c;
    }
    return ext;
}

#define EXTI_CMD 1
#define EXTI_BAT 2
#define EXTI_PS1 3

static std::vector<std::wstring> exts = {L".exe", L".cmd", L".bat", L".ps1"};

std::wstring checkExt(
        const std::vector<std::wstring> &paths,
        const std::wstring &cmd
) {
    auto noExt = true;
    auto ext   = get_ext(wstr_trim(cmd, true, false));

    for (auto &e: exts) {
        if (e == ext) {
            noExt = false;
            break;
        }
    }

    for (auto &p: paths) {
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
        for (auto &e: exts) {
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

void runCmd(std::wstring cmdLine, DWORD &rc, const wchar_t *app = nullptr) {
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

bool ColorShell::run(std::wstring line, std::wstring &cmd, int &rc, std::wstring &err) {
    err.clear();
    std::wstring arg;
    split(line, cmd, arg);
    if (cmd == L"exit") {
        return false;
    } else if (cmd == L"cd") {
        rc = csh::cd(arg);
        if (!rc) { // 目录切换，更新路径
            paths[0] = getCurrentDirectory();
        }
        return true;
    }

    auto t = checkExt(paths, cmd);
    if (t.empty()) {
        err = std::format(L"Unknown command or executable or runnable script file : '{}'", cmd);
    } else if (t == exts[EXTI_CMD] || t == exts[EXTI_BAT]) {
        runCmd(std::format(L"cmd /c {}", line), (DWORD &) rc);
    } else if (t == exts[EXTI_PS1]) {
        runCmd(std::format(L"powershell /c {}", line), (DWORD &) rc);
    } else {
        runCmd(line, (DWORD &) rc, t.c_str());
    }
    return true;
}

#undef EXTI_CMD
#undef EXTI_BAT
#undef EXTI_PS1
