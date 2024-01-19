#include <direct.h>

#include "../../include/inner-cmd.h"
#include "../../include/part/Parts.h"

extern csh::Parts parts;

namespace csh {
    int cd(const str &path) {
        if (path.empty())
            return 0;
        int r = _chdir(path.c_str());
        if (r != 0) {
            std::cerr << "cd: No such directory: '" << path << "'" << std::endl;
        } else {
            parts.cd = true;
            parts.update(csh::UpdateType::WorkDirChanged);
        }
        return r;
    }
}
