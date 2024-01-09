#include <predef.h>
#include <csh-plugin.h>
#include "util.h"
#include "str/wstring-util.h"

extern "C" {
DLL_OUTER_CALL CallResult
CShOnLoadPlugin(std::map<wstr, wstr> &config) {

    return CSH_CALL_FN_SUCCESS;
}

DLL_OUTER_CALL csh::UpdateType

CShUpdateType() {
    return csh::UpdateType::DirModified;
}

DLL_OUTER_CALL bool CShCanShow() {
    try {
        str gitOut = getProcessOutput(L"git rev-parse --is-inside-work-tree");
        return gitOut == "true\n";
    } catch (...) {
    }
    return false;
}

void addModified(std::vector<csh::ColorStrPart> &parts) {

    str modified = getProcessOutput(L"git diff --shortstat");
    modified.erase(modified.begin());
    size_t i = modified.find_first_of(' ');
    if (i == std::string::npos)
        return;
    wstr s = strToWstr(CP_UTF8, modified.substr(0, i));
    parts.push_back(csh::ColorStrPart(std::format(L"\uF4D2 *{} ", s), csh::White));
}

void addBranch(std::vector<csh::ColorStrPart> &parts) {
    str branch = getProcessOutput(L"git rev-parse --abbrev-ref HEAD");
    branch.erase(branch.end() - 1);
    if (branch.length() > 32)
        return;
    parts.push_back(csh::ColorStrPart(std::format(L"\U000f062c {}", strToWstr(CP_UTF8, branch)), csh::White));
}

DLL_OUTER_CALL CallResult
CShOnUpdate(std::vector<csh::ColorStrPart> &parts) {
    parts.clear();
    parts.push_back(csh::ColorStrPart(L" \U000f02a2 ", csh::White));
    addModified(parts);
    addBranch(parts);
    return CSH_CALL_FN_SUCCESS;
}
}
