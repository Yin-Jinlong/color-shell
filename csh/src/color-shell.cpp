#include <color-shell.h>
#include "str/string-util.h"
#include "inner-cmd.h"
#include "util.h"
#include "File.h"
#include "CmdList.h"
#include "csh-fmt.h"
#include <Windows.h>

extern csh::CmdList cmdList;
extern csh::CmdHistory histories;
const std::map<str, CShCmdFn> ColorShell::INNER_CMDS = {
        {ColorShell::CD,             csh::cd},
        {ColorShell::HISTORY,        csh::history},
        {ColorShell::UPDATE_INDEXES, csh::updateIndexes}
};

ColorShell::ColorShell() {
    str path = getEnv("PATH");
    char buf[MAX_PATH];
    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    str mp = buf;
    while (mp[mp.size() - 1] != '\\')
        mp.pop_back();
    // 当前工作目录优先级最高
    paths.push_back(getCurrentDirectory());
    // 其次是程序目录
    paths.push_back(mp);
    // 最后是PATH
    strSplit(path, paths, ';');

    updateIndexes();
}

ColorShell::~ColorShell() = default;

void split(const str &line, ARG_OUT str &cmd, ARG_OUT str &arg) {
    str s = strTrim(line, true, false);
    if (s.empty())
        return;
    size_t p = s.find_first_of(' ');
    if (p == std::string::npos) {
        cmd = s;
        return;
    }
    cmd = s.substr(0, p);
    arg = s.substr(p + 1);
}

#define EXT_I_CMD 1
#define EXT_I_BAT 2
#define EXT_I_PS1 3

str checkExtFile(bool noExt, const str *dir, const str &cmd, const str &ext) {
    if (!noExt) {
        const csh::File &file = dir ? csh::File(*dir, cmd) : csh::File(cmd);
        if (file.exists()) {
            if (ext == ColorShell::EXTS[0]) {
                return file.getPath();
            }
            return ext;
        }
        return "";
    }
    // 补全后缀找
    for (const str &e: ColorShell::EXTS) {
        const csh::File &file = dir ? csh::File(*dir, cmd + e) : csh::File(cmd + e);
        if (file.exists()) {
            if (e == ColorShell::EXTS[0]) {
                return file.getPath();
            }
            return e;
        }
    }
    return "";
}

str checkExt(
        const std::vector<str> &paths,
        const str &cmd
) {
    bool noExt = true;
    str ext = strGetExt(strTrim(cmd, true, false));

    for (const str &e: ColorShell::EXTS) {
        if (e == ext) {
            noExt = false;
            break;
        }
    }

    if (cmd.contains("\\") || cmd.contains("/")) {
        return std::move(checkExtFile(noExt, nullptr, cmd, ext));
    }

    for (const str &p: paths) {
        str r = checkExtFile(noExt, &p, cmd, ext);
        if (!r.empty())
            return std::move(r);
    }
    return "";
}

void runCmd(str cmdLine, DWORD &rc, const char *app = nullptr) {
    PROCESS_INFORMATION pi = {};
    STARTUPINFOA si = {};
    si.cb = sizeof(si);

    if (!CreateProcessA(
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

bool ColorShell::run(str line, str &cmd, int &rc, str &err) {
    err.clear();
    str arg;
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

    str t = checkExt(paths, cmd);
    if (t.empty()) {
        err = csh::format("Unknown command or executable or runnable script file : '{}'", cmd);
    } else {
        bool isCmd = (t == EXTS[EXT_I_CMD] || t == EXTS[EXT_I_BAT]);
        bool isPs = (t == EXTS[EXT_I_PS1]);
        bool isScript = isCmd || isPs;

        str cmdLine = isScript ? (
                isCmd ? csh::format(R"(cmd /c ""{}"" {})", cmd, arg) :
                csh::format("powershell /c {}", line)
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

void filterAdd(std::vector<str> &files, bool cur = false) {
    for (const str &f: files) {
        str ext = strGetExt(f);
        if (ext.empty())
            continue;
        for (const str &e: ColorShell::EXTS) {
            if (e == ext) {
                str cmd = f.substr(0, f.size() - ext.size());
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
    std::vector<str> files;
    str p = paths[0];
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

    std::vector<str> files;
    for (int i = 1; i < paths.size(); i++) {
        str p = paths[i];
        files.clear();
        if (csh::File(p).list(files))
            continue;
        filterAdd(files);
    }
}
