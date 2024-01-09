#include "inner-cmd.h"
#include "color-shell.h"

extern ColorShell sh;

namespace csh {
    int updateIndexes(const wstr &arg) {
        sh.updateIndexes();
        return 0;
    }
}
