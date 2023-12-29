#include <predef.h>
#include <Windows.h>
#include "Console.h"
#include "color-shell.h"

#define C_UTF_8 "C.UTF-8"
#define SET_C_UTF_8(s) s.imbue(std::locale(C_UTF_8))

BOOL WINAPI handleCtrlC(DWORD dwCtrlType);

ColorShell sh;

int main() {
    SetConsoleCtrlHandler(handleCtrlC, TRUE);
    _wsystem(L"chcp 65001>NUL");
    Console::setColorMode(true);

    SET_C_UTF_8(std::wcin);
    SET_C_UTF_8(std::wcout);
    SET_C_UTF_8(std::wcerr);

    int rc = 0;
    while (true) {
        if (rc)
            Console::setForegroundColor(255, 0, 0);
        else
            Console::setForegroundColor(0, 255, 0);
        std::wcout << L"csh> ";

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
