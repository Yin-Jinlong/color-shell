#include <csh-fmt.h>
#include <sstream>

str csh::format(const char *fmt, std::initializer_list<StrValue> args) {
    std::stringstream ss;
    char c;
    const StrValue *arg = args.begin();
    const StrValue *end = args.end();
    while (arg != end) {
        while ((c = *fmt)) {
            if (c == '{' && fmt[1] == '}') {
                fmt += 2;
                ss << arg->value;
                break;
            } else {
                ss << c;
            }
            fmt++;
        }
        arg++;
    }
    ss << fmt;
    return ss.str();
}
