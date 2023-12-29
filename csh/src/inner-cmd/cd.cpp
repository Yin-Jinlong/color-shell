#include <direct.h>
#include "inner-cmd.h"

namespace csh {
    int cd(const std::wstring &path) {
        return _wchdir(path.c_str());
    }
}
