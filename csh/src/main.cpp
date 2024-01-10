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
#include "str/char-util.h"
#include "CmdHistory.h"
#include "CmdList.h"
#include "str/wstring-util.h"

#define SET_UTF_8(s) s.imbue(std::locale(".UTF-8"))

extern void split(const wstr &line, ARG_OUT wstr &cmd, ARG_OUT wstr &arg);

BOOL WINAPI handleCtrlC(DWORD dwCtrlType);

csh::File *historyFile;

csh::CmdList    cmdList;
ColorShell      sh;
csh::Parts      parts;
/**
 * 历史记录
 */
csh::CmdHistory histories;

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

void error(const str &msg) {
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
    histories.load(*historyFile);
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
            histories.save();
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
    pathPartConfig.backgroundColor = csh::Color(224, 192, 80);
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

void mainLoop() {
    int  rc = 0;
    wstr err;

    while (true) {
        int x, y;
        Console::getCursorPosition(x, y);
        if (x) {
            std::wcout << std::endl;
        }

        resetToUTF_8();
        updateParts();
        parts.print();

        Console::reset();
        _flushall();
        Console::save();

        wstr line = input();
        Console::clear();

        if (line.empty()) {
            if (feof(stdin)) {
                std::wcin.clear();
                rc = 0;
            }
            continue;
        }

        wstr cmd;
        try {
            Console::reset();
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

wstr line, hint;
int  hintPos = 0;

void updateHint() {
    hint.clear();
    if (line.empty())
        return;
    for (const wstr &l: histories) {
        if (l.starts_with(line)) {
            hint = l;
            return;
        }
    }
    wstr            cmd, arg;
    split(line, cmd, arg);
    if (arg.empty())
        hint = cmdList.matchOne(cmd);
}

/**
 * 设置光标位置到line的i位置
 */
void setCursorToI(int i) {
    Console::restore();
    int len = i;

    for (int li = 0; li < i; li++) {
        if (is_full_width_char(line[li]))
            len++;
    }
    Console::moveCursorRight(len);
}

bool checkExists(const wstr &cmd) {
    wstr ext = wstrGetExt(cmd);
    if (ext.empty()) {
        for (const wstr &e: ColorShell::EXTS) {
            if (csh::File(cmd + e).exists())
                return true;
        }
    } else {
        return csh::File(cmd).exists();
    }
    return false;
}

/**
 * 重新打印当前输入
 */
void reprint(int i, bool printHint = true) {
    if (printHint) {
        updateHint();
        Console::restore();
        setCursorToI(hintPos);
        Console::clear();
        Console::setForegroundColor(csh::DarkGray);
        Console::print(hint);
        Console::restore();
        Console::setForegroundColor(csh::LightGray);
        Console::print(line);
    }
    wstr cmd, arg;
    split(line, cmd, arg);
    Console::restore();
    if (!printHint)
        Console::clear();
    Console::setForegroundColor((cmdList[cmd] || checkExists(cmd)) ? csh::LightGreen : csh::LightRed);
    Console::print(cmd);
    Console::setForegroundColor(csh::LightGray);
    Console::print(line.substr(cmd.size()));
    setCursorToI(i);
    Console::reset();
}

/**
 * 处理方向键
 */
void dealArrowKey(
        int &i,
        bool &hiChanged,
        const wstr &input,
        u32 key
) {
    if (key == VK_LEFT) {
        i--;
        if (i < 0)
            i = 0;
        else {
            Console::moveCursorLeft(is_full_width_char(line[i]) ? 2 : 1);
        }
    } else if (key == VK_RIGHT) {
        i++;
        if (i > line.size()) {
            if (!hint.empty())
                line = hint;
            i        = static_cast<int>(line.size());
            reprint(i);
        } else {
            Console::moveCursorRight(is_full_width_char(line[i - 1]) ? 2 : 1);
        }
    } else if (key == VK_UP) {
        if (histories.empty())
            return;
        hiChanged = true;
        line      = *histories.last();
        i         = static_cast<int>(line.size());
        reprint(i);
    } else {
        if (histories.empty())
            return;
        hiChanged = true;
        wstr *ns  = histories.next();
        if (ns) {
            line = *ns;
        } else {
            line      = input;
            hiChanged = false;
        }
        i         = static_cast<int>(line.size());
        reprint(i);
    }
}

void complete(
        COORD &sp,
        int &i) {
    if (line.empty())
        return;
    wstr cmd, arg;
    split(line, cmd, arg);
    if (!arg.empty())
        return;
    std::vector<wstr> suggests;
    cmdList.match(cmd, suggests);
    if (suggests.empty())
        return;
    if (suggests.size() == 1) {
        line = suggests[0] + L" ";
        i    = static_cast<int>(line.size());
        reprint(i);
        return;
    }
    Console::println('\r');
    Console::clear();

    COORD size;
    Console::getBufferSize(size);

    // 打印行数
    int      c   = 0;
    // 最大打印行数
    int      pc  = 0;
    // 命令长度
    int      sl  = 0;
    // 下一行起始坐标
    int      off = 0;
    for (int j   = 0; j < suggests.size(); ++j) {
        const wstr &s = suggests[j];
        Console::moveCursorRight(off);
        if (c == 9 && off + sl + 1 >= size.X && j < suggests.size() - 1) {
            Console::println(L"...");
            break;
        }
        Console::println(off + s.length() >= size.X ? s.substr(0, size.X - off - 1) : s);
        if (s.length() > sl)
            sl = static_cast<int>(s.length());
        c++;
        if (pc <= 10)
            pc++;
        if (c >= 10) {
            off += sl + 1;
            sl = 0;
            if (off > size.X)
                break;
            Console::moveCursorUp(c);
            c = 0;
        }
    }

    int d = sp.Y + pc - size.Y;

    if (d > 0) {
        sp.Y = static_cast<SHORT>(sp.Y - d - (pc > 9 ? 1 : 2));
        Console::setCursorPosition(sp);
        Console::save();
    } else {
        Console::setCursorPosition(sp);
    }
    setCursorToI(i);
}


/**
 * 处理单个字符
 */
bool dealChar(
        int &i,
        COORD &sp,
        wchar_t c
) {
    if (c == '\t') {
        complete(sp, i);
    } else if (c == '\r' || c == '\n') {
        reprint(0, false);
        Console::print(L"\r\n");
        return false;
    } else if (c == '\b') {
        if (!line.empty()) {
            if (i > 0) {
                i--;
                line.erase(line.begin() + i);
                reprint(i);
            }
        }
    } else {
        if (c == 3) {
            line = L"";
            return false;
        }
        if (c < ' ')
            return true;
        line.insert(line.begin() + i, c);
        i++;
        reprint(i);
    }
    return true;
}

bool readAllBufChar(int &i, bool &hiChanged, COORD sp, wstr &input) {
    wchar_t c;
    while (!ctrlC && (c = _getwch()) != WEOF) {
        if (!c || c == 0xe0) {
            c = MapVirtualKeyA(_getwch(), MAPVK_VSC_TO_VK);
            if (c == VK_LEFT || c == VK_RIGHT || c == VK_UP || c == VK_DOWN) {
                dealArrowKey(i, hiChanged, input, c);
            } else if (c == VK_DELETE) {
                if (line.empty() || i == line.size())
                    continue;
                line.erase(i, 1);
                reprint(i);
            }
        } else {
            if (!dealChar(i, sp, c))
                return false;
        }
        if (!hiChanged)
            input = line;
    }
    return true;
}

wstr input() {
    ctrlC = false;

    COORD sp;
    Console::getCursorPosition(sp);
    int  i         = 0;
    bool hiChanged = false;

    line.clear();
    wstr input;
    bool inputting = true;

    histories.reset();
    while (inputting) {
        if (_kbhit()) {
            inputting = readAllBufChar(i, hiChanged, sp, input);
        } else if (ctrlC) {
            ctrlC = false;
            return L"";
        }
    }
    histories += line;
    return line;
}
