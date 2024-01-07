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
#include "CmdHistory.h"

#define SET_UTF_8(s) s.imbue(std::locale(".UTF-8"))

BOOL WINAPI handleCtrlC(DWORD dwCtrlType);

csh::File *historyFile;

ColorShell      sh;
csh::Parts      parts;
/**
 * 历史记录
 */
csh::CmdHistory history;

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
wstr input();

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

void error(const str &msg){
    MessageBoxA(nullptr, msg.c_str(), "Error", MB_OK);
    exit(1);
}

void setup() {
    wstr home = getEnv(L"USERPROFILE");
    if (home.empty()) {
        error("Can't find user profile");
    }
    csh::File cshHome(home, L".csh");
    if (!cshHome.exists()) {
        if (!cshHome.mkdirs())
            error("Can't create .csh directory");
    }
    historyFile = new csh::File(cshHome.getPath(), L"history");
    history.load(*historyFile);
}

int main() {
    setup();

    defOut = GetConsoleOutputCP();
    defIn  = GetConsoleCP();
    SetConsoleCtrlHandler(handleCtrlC, TRUE);

    Console::setColorMode(true);

    resetToUTF_8();
    initParts();

    parts.update(csh::UpdateType::Init);

    mainLoop();
    return EXIT_SUCCESS;
}

bool ctrlC = false;

BOOL WINAPI handleCtrlC(DWORD dwCtrlType) {
    if (dwCtrlType == CTRL_C_EVENT) {
        ctrlC = true;
    } else if (dwCtrlType == CTRL_CLOSE_EVENT) {
        if (historyFile)
            history.save();
    }
    return TRUE;
}

void initParts() {
    csh::UserPartConfig userConfig;
    userConfig.backgroundColor = csh::Color(68, 125, 222);
    userConfig.icon            = L"\uF4FF ";
    std::vector<wstr> userContents;
    auto              up = new csh::UserPart(userConfig, userContents);
    parts += up;

    csh::PathPartConfig pathPartConfig;
    pathPartConfig.backgroundColor = csh::Color(240, 205, 100);
    pathPartConfig.icon            = L" \uF413 ";
    pathPartConfig.iconShowMode    = csh::ShowMode::Auto;
    std::vector<wstr> pathContents;
    auto              pp = new csh::PathPart(pathPartConfig, pathContents);
    parts += pp;

    csh::PartConfig nodePluginPartConfig;
    nodePluginPartConfig.backgroundColor = csh::Color(67, 133, 61);
    parts += new csh::PluginPart(nodePluginPartConfig, wstr(L"node"));

    csh::PartConfig gitPluginPartConfig;
    gitPluginPartConfig.backgroundColor = csh::Color(250, 80, 40);
    parts += new csh::PluginPart(gitPluginPartConfig, L"git");

}

void updateParts() {
    csh::File wdir(getCurrentDirectory());
    if (!parts.cd && wdir.lastModified() != parts.lastModifiedTime) {
        parts.update(csh::UpdateType::WorkDirChanged);
        parts.lastModifiedTime = wdir.lastModified();
    } else {
        parts.cd = false;
        parts.update(csh::UpdateType::Update);
    }
}

CONSOLE_SCREEN_BUFFER_INFO cbi = {};

void updateConsoleInfo() {
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cbi);
}

void mainLoop() {
    int  rc = 0;
    wstr err;

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

        wstr line = input();

        if (line.empty()) {
            if (feof(stdin)) {
                std::wcin.clear();
                rc = 0;
            }
            continue;
        }

        wstr cmd;
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
void setCurPos(COORD pos) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/**
 * 设置光标位置
 */
void setCurPos(int x, int y) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {static_cast<SHORT>(x), static_cast<SHORT>(y)});
}

/**
 * 重新打印当前输入
 */
void reprint(const COORD &src, const wstr &line) {
    setCurPos(src);
    Console::clear();
    Console::print(line);
}

/**
 * 设置光标位置到line的i位置
 */
void setCursorToI(const wstr &line, COORD sp, int i) {
    int len = i;

    for (int li = 0; li < i; li++) {
        if (is_full_width_char(line[li]))
            len++;
    }
    setCurPos(sp.X + len, sp.Y);
}

/**
 * 处理方向键
 */
void dealArrowKey(
        wstr &line,
        int &i,
        bool &hiChanged,
        const COORD &sp,
        const wstr &input,
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
            i = static_cast<int>(line.size());
        else {
            Console::moveCursorRight(is_full_width_char(line[i - 1]) ? 2 : 1);
        }
    } else if (key == SK_UP) {
        if (history.empty())
            return;
        hiChanged = true;
        line      = *history.last();
        reprint(sp, line);
        i = static_cast<int>(line.size());
    } else {
        if (history.empty())
            return;
        hiChanged = true;
        wstr *ns  = history.next();
        if (ns) {
            line = *ns;
        } else {
            line      = input;
            hiChanged = false;
        }
        reprint(sp, line);
        i = static_cast<int>(line.size());
    }
}

/**
 * 处理单个字符
 */
bool dealChar(
        wstr &line,
        int &i,
        const COORD &sp,
        wchar_t c
) {
    if (c == '\t') {

    } else if (c == '\r' || c == '\n') {
        Console::print(L"\r\n");
        return false;
    } else if (c == '\b') {
        if (!line.empty()) {
            i--;
            line.erase(line.begin() + i);
            reprint(sp, line);
            setCursorToI(line, sp, i);
        }
    } else {
        if (c < ' ')
            return true;
        line.insert(line.begin() + i, c);
        i++;
        reprint(sp, line);
        setCursorToI(line, sp, i);
    }
    return true;
}

wstr input() {
    ctrlC = false;

    updateConsoleInfo();
    COORD sp        = cbi.dwCursorPosition;
    int   i         = 0;
    bool  hiChanged = false;

    wstr line;
    wstr input;
    bool inputting = true;

    history.reset();
    while (inputting) {
        if (_kbhit()) // 检查是否有按键按下
        {
            wchar_t c = WEOF;
            while ((c = _getwch()) != WEOF) {
                if (!c || c == 0xe0) {
                    c = _getwch();
                    if (c == SK_LEFT || c == SK_RIGHT || c == SK_UP || c == SK_DOWN)
                        dealArrowKey(line, i, hiChanged, sp, input, c);
                    else if (c == SK_DEL) {
                        if (line.empty() || i == line.size())
                            continue;
                        line.erase(i, 1);
                        reprint(sp, line);
                        setCursorToI(line, sp, i);
                    }
                } else {
                    if (!(inputting = dealChar(line, i, sp, c)))
                        break;
                }
                if (!hiChanged)
                    input = line;
            }
        } else if (ctrlC) {
            ctrlC = false;
            return L"";
        }
    }
    history += line;
    return line;
}
