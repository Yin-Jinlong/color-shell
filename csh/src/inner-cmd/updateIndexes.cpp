#include "../../include/inner-cmd.h"
#include "../../include/color-shell.h"

extern ColorShell sh;

namespace csh {
    int updateIndexes(const str &arg) {
        sh.updateIndexes();
        return 0;
    }
}
