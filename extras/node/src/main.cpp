#include <predef.h>
#include <csh-plugin.h>
#include "util.h"
#include "str/wstring-util.h"
#include "File.h"

extern "C" {
DLL_OUTER_CALL CallResult CShOnLoadPlugin(std::map<wstr, wstr> &config) {

    return CSH_CALL_FN_SUCCESS;
}

DLL_OUTER_CALL csh::UpdateType CShUpdateType() {
    return csh::UpdateType::DirModified;
}

DLL_OUTER_CALL bool CShCanShow() {
    wstr      cd  = getCurrentDirectory();
    csh::File dir = csh::File(cd);
    csh::File f(cd + L"/package.json");
    while (!f.exists()) {
        csh::File p = dir.getParent();
        if (dir == p)
            return false;
        dir = p;
        f   = csh::File(dir.getPath() + L"/package.json");
    }
    return true;
}

void addVersion(std::vector<csh::ColorStrPart> &parts) {
    try {
        str v = getProcessOutput(L"node -v");
        v.erase(v.end() - 1);
        v.erase(v.end() - 1);
        parts.push_back(csh::ColorStrPart(strToWstr(CP_UTF8, v), csh::White));
    } catch (...) {
        parts.push_back(csh::ColorStrPart(L"v?.?.?", csh::White));
    }
}

DLL_OUTER_CALL CallResult CShOnUpdate(std::vector<csh::ColorStrPart> &parts) {
    parts.clear();
    parts.push_back(csh::ColorStrPart(L" \U000f0399 ", csh::White));
    addVersion(parts);
    return CSH_CALL_FN_SUCCESS;
}
}
