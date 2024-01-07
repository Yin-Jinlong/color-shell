#include <direct.h>
#include "inner-cmd.h"
#include "part/Parts.h"
#include "str/wstring-util.h"
#include "Console.h"

extern csh::Parts parts;

namespace csh {
    int cd(const wstr &path) {
        if (path.empty())
            return 0;
        int r = _wchdir(path.c_str());
        if (r != 0) {
            std::wcerr << L"cd: No such directory: '" << path << L"'" << std::endl;
        } else {
            parts.cd = true;
            parts.update(csh::UpdateType::WorkDirChanged);
        }
        return r;
    }

    int history(CmdHistory &history, const wstr &argLine) {
        wstr s = wstr_trim(argLine);
        if (s.empty()) {
            Console::printf(L"history: {}", history.length());
            return 0;
        }
        std::vector<wstr> args;
        wstr_split(s, args, ' ');
        if (args[0] != L"-c") {
            std::wcerr << L"Usage: history [-c]" << std::endl;
            return 1;
        }
        history.clear();
        history.save();
        return 0;
    }
}
