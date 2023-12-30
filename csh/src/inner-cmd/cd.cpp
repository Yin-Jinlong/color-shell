#include <direct.h>
#include "inner-cmd.h"
#include "part/Parts.h"

extern csh::Parts parts;

namespace csh {
    int cd(const std::wstring &path) {
        if (path.empty())
            return 0;
        auto r = _wchdir(path.c_str());
        if (r != 0) {
            std::wcerr << L"cd: No such directory: '" << path << L"'" << std::endl;
        } else {
            parts.cd = true;
            parts.update(csh::UpdateType::WORK_DIR_CHANGED);
        }
        return r;
    }
}
