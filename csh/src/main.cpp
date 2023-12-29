#include <predef.h>
#include <Windows.h>
#include "Console.h"
#include "color-shell.h"
#include "part/PathPart.h"
#include "part/Parts.h"

#define SET_C_UTF_8(s) s.imbue(std::locale("en.UTF-8"))

BOOL WINAPI handleCtrlC(DWORD dwCtrlType);

ColorShell sh;
csh::Parts parts;

void initParts();

int main() {
    SetConsoleCtrlHandler(handleCtrlC, TRUE);
    _wsystem(L"chcp 65001>NUL");
    Console::setColorMode(true);

    SET_C_UTF_8(std::wcin);
    SET_C_UTF_8(std::wcout);
    SET_C_UTF_8(std::wcerr);

    initParts();

    int rc = 0;
    while (true) {
        parts.update();
        parts.print();

        wchar_t buf[1024];
        Console::reset();
        std::wcin.getline(buf, 1024);
        std::wstring cmdLine(buf);

        if (cmdLine.empty()) {
            if (feof(stdin)) {
                std::wcin.clear();
                rc = 0;
            }
            continue;
        }

        std::wstring cmd;
        try {
            if (sh.run(cmdLine, cmd, rc))
                break;
        } catch (std::runtime_error &err) {
            std::wcout << L"Unknown command or executable : " << cmd << std::endl;
        }
    }
    return EXIT_SUCCESS;
}

BOOL WINAPI handleCtrlC(DWORD dwCtrlType) {
    return TRUE;
}

void initParts() {
    auto pathPartConfig = csh::PathPartConfig(
            csh::White,
            csh::Color(240, 205, 100),
            L" \uF413 ",
            csh::ShowMode::AUTO
    );
    auto vector1 = std::vector<std::wstring>();
    csh::PathPart pp(pathPartConfig, vector1);
    parts += &pp;
}
