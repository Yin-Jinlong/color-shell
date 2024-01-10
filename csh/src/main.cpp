#include <predef.h>
#include <Windows.h>
#include <conio.h>
#include <io.h>
#include <fstream>
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
#include "str/string-util.h"
//#include "yaml-cpp/yaml.h"

#define SET_UTF_8(s) s.imbue(std::locale(".UTF-8"))

extern void split(const str &line, ARG_OUT str &cmd, ARG_OUT str &arg);

BOOL WINAPI handleCtrlC(DWORD dwCtrlType);

//csh::UserPartConfig userConfig;

csh::File *historyFile;

csh::CmdList    cmdList;
ColorShell      sh;
csh::Parts      parts;
/**
 * 历史记录
 */
csh::CmdHistory histories;

str line, hint;
int hintPos = 0;

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
void input();

void printlnShortLine();

u32 defOut, defIn;

/**
 * 重置编码到 UTF-8
 */
void resetToUTF_8() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    SET_UTF_8(std::cin);
    SET_UTF_8(std::cout);
    SET_UTF_8(std::cerr);
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

void createFile(const str &file) {
    std::wofstream f(file);
    if (!f.is_open()) {
        error("Can't create file");
    }
    f.close();
}

void mkdir(str &home) {
    csh::File f(home, ".csh");
    if (f.exists())
        return;
    if (!f.mkdirs())
        error("Can't create .csh directory");
}

void setup() {
    str home = getEnv("USERPROFILE");
    if (home.empty()) {
        error("Can't find user profile");
    }
    mkdir(home);
    historyFile = new csh::File(home, "csh/history");
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
    userConfig.icon            = "\uF4FF ";
    std::vector<str> userContents;
    auto             up = new csh::UserPart(userConfig, userContents);
    parts += up;

    csh::PathPartConfig pathPartConfig;
    pathPartConfig.backgroundColor = csh::Color(224, 192, 80);
    pathPartConfig.icon            = " \uF413 ";
    pathPartConfig.iconShowMode    = csh::ShowMode::Auto;
    std::vector<str> pathContents;
    auto             pp = new csh::PathPart(pathPartConfig, pathContents);
    parts += pp;

    csh::PartConfig nodePluginPartConfig;
    nodePluginPartConfig.backgroundColor = csh::Color(67, 133, 61);
    parts += new csh::PluginPart(nodePluginPartConfig, str("node"));

    csh::PartConfig gitPluginPartConfig;
    gitPluginPartConfig.backgroundColor = csh::Color(250, 80, 40);
    parts += new csh::PluginPart(gitPluginPartConfig, "git");

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
    int rc = 0;
    str err;

    while (true) {
        int x, y;
        Console::getCursorPosition(x, y);
        if (x) {
            Console::println();
        }

        resetToUTF_8();
        updateParts();
        parts.print();

        Console::reset();
        _flushall();
        Console::save();

        input();

        if (line.empty()) {
            if (feof(stdin)) {
                std::wcin.clear();
                rc = 0;
            }
            ctrlC = true;
        }
        if (ctrlC) {
            printlnShortLine();
            continue;
        }

        str cmd;
        try {
            Console::reset();
            resetToDefault();
            if (!sh.run(line, cmd, rc, err))
                break;
            if (!err.empty()) {
                resetToUTF_8();
                Console::setForegroundColor(csh::Red);
                std::cerr << "error: ";
                Console::reset();
                std::cerr << err << std::endl;
            }
        } catch (std::runtime_error &err) {
            MessageBoxA(nullptr, err.what(), "error", MB_OK);
        }
    }
}

void updateHint() {
    hint.clear();
    if (line.empty())
        return;
    for (const str &l: histories) {
        if (l.starts_with(line)) {
            hint = l;
            return;
        }
    }
    str            cmd, arg;
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
        if (isFullWidthChar(line[li]))
            len++;
    }
    Console::moveCursorRight(len);
}

bool checkExists(const str &cmd) {
    str ext = strGetExt(cmd);
    if (ext.empty()) {
        for (const str &e: ColorShell::EXTS) {
            if (csh::File(cmd + e).exists())
                return true;
        }
    } else {
        return csh::File(cmd).exists();
    }
    return false;
}

void printCmdLine() {
    str cmd, arg;
    split(line, cmd, arg);
    Console::setForegroundColor((cmdList[cmd] || checkExists(cmd)) ? csh::LightGreen : csh::LightRed);
    Console::print(cmd);
    Console::setForegroundColor(csh::LightGray);
    Console::print(line.substr(cmd.size()));
}

void printlnShortLine() {
    Console::print('\r');
    Console::print("csh>");
    printCmdLine();
    Console::clear();
    Console::println();
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
    Console::restore();
    if (!printHint)
        Console::clear();
    printCmdLine();
    setCursorToI(i);
    Console::reset();
}

/**
 * 处理方向键
 */
void dealArrowKey(
        int &i,
        bool &hiChanged,
        const str &input,
        u32 key
) {
    if (key == VK_LEFT) {
        i--;
        if (i < 0)
            i = 0;
        else {
            Console::moveCursorLeft(isFullWidthChar(line[i]) ? 2 : 1);
        }
    } else if (key == VK_RIGHT) {
        i++;
        if (i > line.size()) {
            if (!hint.empty())
                line = hint;
            i        = static_cast<int>(line.size());
            reprint(i);
        } else {
            Console::moveCursorRight(isFullWidthChar(line[i - 1]) ? 2 : 1);
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
        str *ns   = histories.next();
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
    str cmd, arg;
    split(line, cmd, arg);
    if (!arg.empty())
        return;
    std::vector<str> suggests;
    cmdList.match(cmd, suggests);
    if (suggests.empty())
        return;
    if (suggests.size() == 1) {
        line = suggests[0] + " ";
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
        const str &s = suggests[j];
        Console::moveCursorRight(off);
        if (c == 9 && off + sl + 1 >= size.X && j < suggests.size() - 1) {
            Console::println("...");
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
        int c
) {
    if (c == '\t') {
        complete(sp, i);
    } else if (c == '\r' || c == '\n') {
        printlnShortLine();
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
            ctrlC = true;
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

bool readAllBufChar(int &i, bool &hiChanged, COORD sp, str &input) {
    int c;
    while (!ctrlC && (c = _getch()) != EOF) {
        if (!c || c == 0xe0) {
            u32 key = MapVirtualKeyA(_getch(), MAPVK_VSC_TO_VK);
            if (key == VK_LEFT || key == VK_RIGHT || key == VK_UP || key == VK_DOWN) {
                dealArrowKey(i, hiChanged, input, key);
            } else if (key == VK_DELETE) {
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

void input() {
    ctrlC = false;

    COORD sp;
    Console::getCursorPosition(sp);
    int  i         = 0;
    bool hiChanged = false;

    line.clear();
    str  input;
    bool inputting = true;

    histories.reset();
    while (inputting) {
        if (_kbhit()) {
            inputting = readAllBufChar(i, hiChanged, sp, input);
        } else if (ctrlC) {
            return;
        }
    }
    histories += line;
}
