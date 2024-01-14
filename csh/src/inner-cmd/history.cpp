#include "inner-cmd.h"
#include "part/Parts.h"
#include "str/string-util.h"
#include "Console.h"
#include "color-shell.h"

extern csh::CmdHistory histories;

namespace csh {

    int history(const str &arg) {
        str s = strTrim(arg);
        if (s.empty()) {
            Console::printf("histories: {}", {histories.length()});
            return 0;
        }
        std::vector<str> args;
        strSplit(s, args, ' ');
        if (args[0] != "-c") {
            std::cerr << "Usage: histories [-c]" << std::endl;
            return 1;
        }
        histories.clear();
        histories.save();
        return 0;
    }
}
