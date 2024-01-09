#include "inner-cmd.h"
#include "part/Parts.h"
#include "str/wstring-util.h"
#include "Console.h"
#include "color-shell.h"

extern csh::CmdHistory histories;

namespace csh {

    int history(const wstr &arg) {
        wstr s = wstrTrim(arg);
        if (s.empty()) {
            Console::printf(L"histories: {}", histories.length());
            return 0;
        }
        std::vector<wstr> args;
        wstrSplit(s, args, ' ');
        if (args[0] != L"-c") {
            std::wcerr << L"Usage: histories [-c]" << std::endl;
            return 1;
        }
        histories.clear();
        histories.save();
        return 0;
    }
}
