#include <color-shell.h>
#include "str/wstring-util.h"
#include "inner-cmd.h"
#include "util.h"
#include "File.h"
#include "CmdList.h"
#include <Windows.h>

extern csh::CmdList            cmdList;
extern csh::CmdHistory         histories;
const std::map<wstr, CShCmdFn> ColorShell::INNER_CMDS = {
        {ColorShell::CD,             csh::cd},
        {ColorShell::HISTORY,        csh::history},
        {ColorShell::UPDATE_INDEXES, csh::updateIndexes}
};

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
    wstrSplit(path, paths, ';');

    updateIndexes();
}

ColorShell::~ColorShell() = default;

void split(const wstr &line, ARG_OUT wstr &cmd, ARG_OUT wstr &arg) {
    wstr s = wstrTrim(line, true, false);
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

#define EXT_I_CMD 1
#define EXT_I_BAT 2
#define EXT_I_PS1 3

wstr checkExtFile(bool noExt, const wstr *dir, const wstr &cmd, const wstr &ext) {
    if (!noExt) {
        const csh::File &file = dir ? csh::File(*dir, cmd) : csh::File(cmd);
        if (file.exists()) {
            if (ext == ColorShell::EXTS[0]) {
                return file.getPath();
            }
            return ext;
        }
        return L"";
    }
    // 补全后缀找
    for (const wstr &e: ColorShell::EXTS) {
        const csh::File &file = dir ? csh::File(*dir, cmd + e) : csh::File(cmd + e);
        if (file.exists()) {
            if (e == ColorShell::EXTS[0]) {
                return file.getPath();
            }
            return e;
        }
    }
    return L"";
}

wstr checkExt(
        const std::vector<wstr> &paths,
        const wstr &cmd
) {
    bool noExt = true;
    wstr ext   = wstrGetExt(wstrTrim(cmd, true, false));

    for (const wstr &e: ColorShell::EXTS) {
        if (e == ext) {
            noExt = false;
            break;
        }
    }

    if (cmd.contains(L"\\") || cmd.contains(L"/")) {
        return std::move(checkExtFile(noExt, nullptr, cmd, ext));
    }

    for (const wstr &p: paths) {
        wstr r = checkExtFile(noExt, &p, cmd, ext);
        if (!r.empty())
            return std::move(r);
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
    if (cmd == EXIT) {
        return false;
    }
    auto cmdFn = INNER_CMDS.find(cmd);
    if (cmdFn != INNER_CMDS.end()) {
        rc = cmdFn->second(arg);
        if (cmd == CD && !rc) {// 目录切换，更新路径
            paths[0] = getCurrentDirectory();
            updateCurrentIndexes();
        }
        return true;
    }

    wstr t = checkExt(paths, cmd);
    if (t.empty()) {
        err = std::format(L"Unknown command or executable or runnable script file : '{}'", cmd);
    } else {
        bool isCmd    = (t == EXTS[EXT_I_CMD] || t == EXTS[EXT_I_BAT]);
        bool isPs     = (t == EXTS[EXT_I_PS1]);
        bool isScript = isCmd || isPs;

        wstr cmdLine = isScript ? (
                isCmd ? std::format(L"cmd /c \"\"{}\"\" {}", cmd, arg) :
                std::format(L"powershell /c {}", line)
        ) : line;// 是exe

        runCmd(cmdLine,
               (DWORD &) rc,
               isScript ? nullptr : t.c_str());
    }
    return true;
}

#undef EXTI_CMD
#undef EXTI_BAT
#undef EXTI_PS1

void filterAdd(std::vector<wstr> &files, bool cur = false) {
    for (const wstr &f: files) {
        wstr ext = wstrGetExt(f);
        if (ext.empty())
            continue;
        for (const wstr &e: ColorShell::EXTS) {
            if (e == ext) {
                wstr cmd = f.substr(0, f.size() - ext.size());
                if (cur) {
                    cmdList.addCurrent(cmd);
                } else {
                    cmdList += cmd;
                }
                break;
            }
        }
    }
}

void ColorShell::updateCurrentIndexes() {
    cmdList.clearCurrent();
    std::vector<wstr> files;
    wstr              p = paths[0];
    if (csh::File(p).list(files))
        return;
    filterAdd(files, true);
}

void ColorShell::updateIndexes() {
    cmdList.clear();
    updateCurrentIndexes();
    cmdList += CD;
    cmdList += EXIT;
    cmdList += HISTORY;
    cmdList += UPDATE_INDEXES;

    std::vector<wstr> files;
    for (int          i = 1; i < paths.size(); i++) {
        wstr p = paths[i];
        files.clear();
        if (csh::File(p).list(files))
            continue;
        filterAdd(files);
    }
}
