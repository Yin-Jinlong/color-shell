#include <predef.h>
#include <Windows.h>
#include "Console.h"
#include "color-shell.h"
#include "part/PathPart.h"
#include "part/Parts.h"
#include "part/UserPart.h"
#include "part/PluginPart.h"
#include "file.h"
#include "util.h"

#define SET_UTF_8(s) s.imbue(std::locale(".UTF-8"))

BOOL WINAPI handleCtrlC(DWORD dwCtrlType);

ColorShell sh;
csh::Parts parts;

void initParts();

int main() {
    SetConsoleCtrlHandler(handleCtrlC, TRUE);
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    Console::setColorMode(true);

    SET_UTF_8(std::wcin);
    SET_UTF_8(std::wcout);
    SET_UTF_8(std::wcerr);

    initParts();

    parts.update(csh::UpdateType::INIT);

    int rc = 0;
    while (true) {
        CONSOLE_SCREEN_BUFFER_INFO ocbi = {}, icbi = {};
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ocbi);
        GetConsoleScreenBufferInfo(GetStdHandle(STD_INPUT_HANDLE), &icbi);
        if (ocbi.dwCursorPosition.X || icbi.dwCursorPosition.X) {
            std::wcout << std::endl;
        }

        csh::File wdir(getCurrentDirectory());
        if (!parts.cd && wdir.lastModified() != parts.lastModifiedTime) {
            parts.update(csh::UpdateType::WORK_DIR_CHANGED);
            parts.lastModifiedTime = wdir.lastModified();
        } else {
            parts.cd = false;
            parts.update(csh::UpdateType::UPDATE);
        }

        parts.print();

        wchar_t buf[4096];
        Console::reset();
        _flushall();
        std::wcin.getline(buf, 4096);
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
    csh::UserPartConfig userConfig;
    userConfig.backgroundColor = csh::Color(68, 125, 222);
    userConfig.icon = L"\uF4FF ";
    auto userContents = std::vector<std::wstring>();
    auto up = new csh::UserPart(userConfig, userContents);
    parts += up;

    csh::PathPartConfig pathPartConfig;
    pathPartConfig.backgroundColor = csh::Color(240, 205, 100);
    pathPartConfig.icon = L" \uF413 ";
    pathPartConfig.iconShowMode = csh::ShowMode::AUTO;
    auto pathContents = std::vector<std::wstring>();
    auto pp = new csh::PathPart(pathPartConfig, pathContents);
    parts += pp;

    std::wstring pluginName = L"git";
    csh::PartConfig pluginPartConfig;
    pluginPartConfig.backgroundColor = csh::Color(250, 80, 40);
    auto plugin = new csh::PluginPart(pluginPartConfig, pluginName);
    parts += plugin;

}
