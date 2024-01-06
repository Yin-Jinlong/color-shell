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
#include "str/char-util.h"

#define SET_UTF_8(s) s.imbue(std::locale(".UTF-8"))

BOOL WINAPI handleCtrlC(DWORD dwCtrlType);

ColorShell sh;
csh::Parts parts;

/**
 * 初始化
 */
void initParts();

/**
 * 更新
 */
void updateParts();

void mainLoop();

/**
 * 处理输入
 */
std::wstring input();

u32 defOut, defIn;

/**
 * 重置编码到 UTF-8
 */
void resetToUTF_8() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    SET_UTF_8(std::wcin);
    SET_UTF_8(std::wcout);
    SET_UTF_8(std::wcerr);
}

/**
 * 重置编码到默认
 */
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
    int          rc = 0;
    std::wstring err;

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
            if (!sh.run(line, cmd, rc, err))
                break;
            if (!err.empty()) {
                resetToUTF_8();
                Console::setForegroundColor(csh::Red);
                std::wcerr << L"error: ";
                Console::reset();
                std::wcerr << err << std::endl;
            }
        } catch (std::runtime_error &err) {
            MessageBoxA(nullptr, err.what(), "error", MB_OK);
        }
    }
}

/**
 * 设置光标位置
 */
void setCurPos(_COORD pos) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/**
 * 设置光标位置
 */
void setCurPos(SHORT x, SHORT y) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {x, y});
}

/**
 * 历史记录
 */
std::vector<std::wstring> history;

/**
 * 重新打印当前输入
 */
void reprint(const _COORD &src, const std::wstring &line) {
    setCurPos(src);
    Console::clear();
    Console::print(line);
}

/**
 * 设置光标位置到line的i位置
 */
void SetCursorToI(const std::wstring &line, _COORD sp, short i) {
    int len = i;

    for (auto li=0;li<i;li++) {
        if (is_full_width_char(line[li]))
            len++;
    }
    setCurPos(sp.X + len, sp.Y);
}

/**
 * 处理方向键
 */
void dealArrowKey(
        std::wstring &line,
        int &i,
        int &hi,
        bool &hiChanged,
        const _COORD &sp,
        const std::wstring &input,
        int key
) {
    if (key == SK_LEFT) {
        i--;
        if (i < 0)
            i = 0;
        else {
            Console::moveCursorLeft(is_full_width_char(line[i]) ? 2 : 1);
        }
    } else if (key == SK_RIGHT) {
        i++;
        if (i > line.size())
            i = line.size();
        else {
            Console::moveCursorRight(is_full_width_char(line[i - 1]) ? 2 : 1);
        }
    } else if (key == SK_UP) {
        if (history.empty())
            return;
        if (hi > 0)
            hi--;
        hiChanged = true;
        line      = history[hi];
        reprint(sp, line);
        i = line.size();
    } else {
        if (history.empty())
            return;
        hiChanged = true;
        if (hi < history.size())
            hi++;
        if (hi == history.size()) {
            line      = input;
            hiChanged = false;
        } else {
            line = history[hi];
        }
        reprint(sp, line);
        i = line.size();
    }
}

/**
 * 处理单个字符
 */
bool dealChar(
        std::wstring &line,
        std::wstring &input,
        int &i,
        int &hi,
        bool &hiChanged,
        const _COORD &sp,
        wchar_t c
) {
    if (!c || c == 0xe0) {
        c = _getwch();
        if (c == SK_LEFT || c == SK_RIGHT || c == SK_UP || c == SK_DOWN)
            dealArrowKey(line, i, hi, hiChanged, sp, input, c);
        else if (c == SK_DEL) {
            if (line.empty() || i == line.size())
                return true;
            line.erase(i, 1);
            reprint(sp, line);
            SetCursorToI(line, sp, i);
        }
    } else {
        if (c == '\t') {

        } else if (c == '\r' || c == '\n') {
            Console::print(L"\r\n");
            return false;
        } else if (c == '\b') {
            if (!line.empty()) {
                i--;
                line.erase(line.begin() + i);
                reprint(sp, line);
                SetCursorToI(line, sp, i);
            }
        } else {
            if (c < ' ')
                return true;
            line.insert(line.begin() + i, c);
            i++;
            reprint(sp, line);
            SetCursorToI(line, sp, i);
        }
    }
    if (!hiChanged)
        input = line;
    return true;
}

std::wstring input() {
    ctrlC = false;

    updateConsoleInfo();
    _COORD sp        = cbi.dwCursorPosition;
    int    i         = 0;
    int    hi        = history.size();
    bool   hiChanged = false;

    std::wstring line;
    std::wstring input;
    bool         inputting = true;

    while (inputting) {
        if (_kbhit()) // 检查是否有按键按下
        {
            wchar_t c = WEOF;
            while ((c = _getwch()) != WEOF) {
                if (!(inputting = dealChar(line, input, i, hi, hiChanged, sp, c)))
                    break;
            }
        } else if (ctrlC) {
            ctrlC = false;
            return L"";
        }
    }
    if (history.empty() || history[history.size() - 1] != line)
        history.push_back(line);
    return line;
}
