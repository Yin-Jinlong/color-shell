#include <predef.h>
#include <csh-plugin.h>
#include "util.h"
#include "str/wstring-util.h"
#include "File.h"

extern "C" {
DLL_OUT CallResult CShOnLoadPlugin(std::map<std::wstring, std::wstring> &config) {

    return CSH_CALL_FN_SUCCESS;
}

DLL_OUT csh::UpdateType CShUpdateTime() {
    return csh::UpdateType::DIR_MODIFIED;
}

DLL_OUT bool CShCanShow() {
    auto cd = getCurrentDirectory();
    auto dir = csh::File(cd);
    csh::File f(cd + L"/package.json");
    while (!f.exists()) {
        auto p = dir.getParent();
        if (dir == p)
            return false;
        dir = p;
        f = csh::File(dir.getPath() + L"/package.json");
    }
    return true;
}

DLL_OUT CallResult CShOnUpdate(std::vector<csh::ColorStrPart> &parts) {
    parts.clear();
    parts.push_back(csh::ColorStrPart(L" \U000f0399 ", csh::White));
    return CSH_CALL_FN_SUCCESS;
}
}
