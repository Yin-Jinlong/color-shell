#include <predef.h>
#include <Windows.h>
#include <conio.h>
#include "Console.h"
#include "color-shell.h"
#include "part/PathPart.h"
#include "part/Parts.h"
#include "part/UserPart.h"
#include "part/PluginPart.h"
#include "file.h"
#include "util.h"
#include "key-scan-code.h"

#define SET_UTF_8(s) s.imbue(std::locale(".UTF-8"))

BOOL WINAPI handleCtrlC(DWORD dwCtrlType);

ColorShell sh;
csh::Parts parts;

void initParts();

void updateParts();

void mainLoop();

std::wstring input();

u32 defOut, defIn;

void resetToUTF_8() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    SET_UTF_8(std::wcin);
    SET_UTF_8(std::wcout);
    SET_UTF_8(std::wcerr);
}

void resetToDefault() {
    SetConsoleCP(defIn);
    SetConsoleOutputCP(defOut);
}

int main() {
    defOut = GetConsoleOutputCP();
    defIn  = GetConsoleCP();
    SetConsoleCtrlHandler(handleCtrlC, TRUE);

    Console::setColorMode(true);

    resetToUTF_8();
    initParts();

    parts.update(csh::UpdateType::INIT);

    mainLoop();
    return EXIT_SUCCESS;
}

bool ctrlC = false;

BOOL WINAPI handleCtrlC(DWORD dwCtrlType) {
    if (dwCtrlType == CTRL_C_EVENT) {
        ctrlC = true;
    }
    return TRUE;
}

void initParts() {
    csh::UserPartConfig userConfig;
    userConfig.backgroundColor = csh::Color(68, 125, 222);
    userConfig.icon            = L"\uF4FF ";
    auto userContents = std::vector<std::wstring>();
    auto up           = new csh::UserPart(userConfig, userContents);
    parts += up;

    csh::PathPartConfig pathPartConfig;
    pathPartConfig.backgroundColor = csh::Color(240, 205, 100);
    pathPartConfig.icon            = L" \uF413 ";
    pathPartConfig.iconShowMode    = csh::ShowMode::AUTO;
    auto pathContents = std::vector<std::wstring>();
    auto pp           = new csh::PathPart(pathPartConfig, pathContents);
    parts += pp;

    csh::PartConfig nodePluginPartConfig;
    nodePluginPartConfig.backgroundColor = csh::Color(67, 133, 61);
    parts += new csh::PluginPart(nodePluginPartConfig, std::wstring(L"node"));

    csh::PartConfig gitPluginPartConfig;
    gitPluginPartConfig.backgroundColor = csh::Color(250, 80, 40);
    parts += new csh::PluginPart(gitPluginPartConfig, L"git");

}

void updateParts() {
    csh::File wdir(getCurrentDirectory());
    if (!parts.cd && wdir.lastModified() != parts.lastModifiedTime) {
        parts.update(csh::UpdateType::WORK_DIR_CHANGED);
        parts.lastModifiedTime = wdir.lastModified();
    } else {
        parts.cd = false;
        parts.update(csh::UpdateType::UPDATE);
    }
}

CONSOLE_SCREEN_BUFFER_INFO cbi = {};

void updateConsoleInfo() {
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cbi);
}

void mainLoop() {
    int rc = 0;
    while (true) {
        updateConsoleInfo();
        if (cbi.dwCursorPosition.X) {
            std::wcout << std::endl;
        }

        resetToUTF_8();
        updateParts();
        parts.print();

        Console::reset();
        _flushall();

        std::wstring line = input();

        if (line.empty()) {
            if (feof(stdin)) {
                std::wcin.clear();
                rc = 0;
            }
            continue;
        }

        std::wstring cmd;
        try {
            resetToDefault();
            if (sh.run(line, cmd, rc))
                break;
        } catch (std::runtime_error &err) {
            std::wcerr << L"error: " << err.what() << std::endl;
        }
    }
}

void setCurPos(_COORD pos) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setCurPos(SHORT x, SHORT y) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {x, y});
}

std::vector<std::wstring> history;

std::wstring input() {
    ctrlC = false;

    updateConsoleInfo();
    _COORD sp        = cbi.dwCursorPosition;
    short  i         = 0;
    int    hi        = history.size();
    bool   hiChanged = false;

    std::wstring line;
    std::wstring input;
    while (true) {
        if (_kbhit()) // 检查是否有按键按下
        {
            wchar_t c = _getwch();
            if (!c || c == 0xe0) {
                switch (_getwch()) {
                    case SK_LEFT:
                        i--;
                        if (i < 0)
                            i = 0;
                        else
                            Console::moveCursorLeft(1);
                        break;
                    case SK_RIGHT:
                        i++;
                        if (i > line.size())
                            i = line.size();
                        else
                            Console::moveCursorRight(1);
                        break;
                    case SK_UP:
                        if (history.empty())
                            break;
                        if (hi > 0)
                            hi--;
                        hiChanged = true;
                        line = history[hi];
                        setCurPos(sp);
                        Console::clear();
                        Console::print(line);
                        i = line.size();
                        break;
                    case SK_DOWN:
                        if (history.empty())
                            break;
                        hiChanged = true;
                        if (hi < history.size())
                            hi++;
                        if (hi == history.size()) {
                            line      = input;
                            hiChanged = false;
                        } else {
                            line = history[hi];
                        }
                        setCurPos(sp);
                        Console::clear();
                        Console::print(line);
                        i = line.size();
                        break;
                }
            } else {
                if (c == '\t') {
                    Console::print(' ');
                } else if (c == '\r' || c == '\n') {
                    Console::print(L"\r\n");
                    break;
                } else if (c == '\b') {
                    if (!line.empty()) {
                        i--;
                        line.erase(line.begin() + i);
                        setCurPos(sp);
                        Console::clear();
                        Console::print(line);
                        setCurPos(sp.X + i, sp.Y);
                    }
                } else {
                    if (c < ' ')
                        continue;
                    line.insert(line.begin() + i, c);
                    i++;
                    setCurPos(sp);
                    Console::print(line);
                    setCurPos(sp.X + i, sp.Y);
                }
            }
            if (!hiChanged)
                input = line;
        } else if (ctrlC) {
            ctrlC = false;
            return L"";
        }
    }
    if (history.empty() || history[history.size() - 1] != line)
        history.push_back(line);
    return line;
}
