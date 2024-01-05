#include <color-shell.h>
#include "str/wstring-util.h"
#include "inner-cmd.h"
#include "util.h"
#include "File.h"
#include <Windows.h>
#include <algorithm>

ColorShell::ColorShell() {
    auto path = getEnv(L"PATH");
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
    cmd = s.substr(0, p);
    arg = s.substr(p + 1);
}

std::wstring get_ext(const std::wstring &path) {
    auto p = path.find_last_of('.');
    if (p == std::wstring::npos)
        return L"";
    auto ext = path.substr(p);
    for (int i = 0; i < ext.length(); ++i) {
        auto c = ext[i];
        if (c >= 'A' && c <= 'Z')
            c -= 'A' - 'a';
        ext[i] = c;
    }
    return ext;
}

bool checkExt(
        const std::vector<std::wstring> &paths,
        std::initializer_list<std::wstring> exts,
        const std::wstring &cmd
) {
    auto noExt = true;
    auto ext = get_ext(wstr_trim(cmd, true, false));
    for (auto &e: exts) {
        if (e == ext) {
            noExt = false;
            break;
        }
    }
    // 路径中存在该命令
    return std::any_of(paths.begin(), paths.end(), [&](const std::wstring &p) {
        // 查找所有后缀
        return noExt ? std::any_of(exts.begin(), exts.end(), [&](const std::wstring &e) {
            csh::File file(p, cmd + e);
            return file.exists();
        }) : csh::File(p, cmd).exists();
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
    static std::initializer_list<std::wstring> exeExts = {L".exe"};
    static std::initializer_list<std::wstring> cmdExts = {L".cmd", L".bat"};
    static std::initializer_list<std::wstring> psExts = {L".ps1"};

    std::wstring arg;
    split(line, cmd, arg);
    if (cmd == L"exit")
        return true;
    if (cmd == L"cd") {
        rc = csh::cd(arg);
        if (!rc) { // 目录切换，更新路径
            paths[0] = getCurrentDirectory();
        }
    } else if (checkExt(paths, exeExts, cmd)) { // exe 优先级最高
        runCmd(line, (DWORD &) rc);
    } else if (checkExt(paths, cmdExts, cmd)) { // cmd脚本
        runCmd(std::format(L"cmd /c {}", line), (DWORD &) rc);
    } else if (checkExt(paths, psExts, cmd)) { // powershell脚本
        runCmd(std::format(L"powershell /c {}", line), (DWORD &) rc);
    } else {
        char buf[MAX_PATH];
        WideCharToMultiByte(CP_UTF8, 0, cmd.data(), -1, buf, MAX_PATH, nullptr, nullptr);
        throw std::runtime_error(
                std::format("Unknown command or executable or runnable script file : '{}'", buf));
    }
    return false;
}

