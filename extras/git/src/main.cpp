#include <predef.h>
#include <csh-plugin.h>
#include "util.h"
#include "str/string-util.h"

extern "C" {
DLL_OUTER_CALL CallResult
CShOnLoadPlugin(std::map<str, str> &config) {

    return CSH_CALL_FN_SUCCESS;
}

DLL_OUTER_CALL csh::UpdateType

CShUpdateType() {
    return csh::UpdateType::DirModified;
}

DLL_OUTER_CALL bool CShCanShow() {
    try {
        str gitOut = getProcessOutput("git rev-parse --is-inside-work-tree");
        return gitOut == "true\n";
    } catch (...) {
    }
    return false;
}

void addModified(std::vector<csh::ColorStrPart> &parts) {

    str modified = getProcessOutput("git diff --shortstat");
    modified.erase(modified.begin());
    size_t i = modified.find_first_of(' ');
    if (i == std::string::npos)
        return;
    parts.push_back(csh::ColorStrPart(std::format("\uF4D2 *{} ", modified.substr(0, i)), csh::White));
}

void addBranch(std::vector<csh::ColorStrPart> &parts) {
    str branch = getProcessOutput("git rev-parse --abbrev-ref HEAD");
    branch.erase(branch.end() - 1);
    if (branch.length() > 32)
        return;
    parts.push_back(csh::ColorStrPart(std::format("\U000f062c {}", branch), csh::White));
}

DLL_OUTER_CALL CallResult
CShOnUpdate(std::vector<csh::ColorStrPart> &parts) {
    parts.clear();
    parts.push_back(csh::ColorStrPart(" \U000f02a2 ", csh::White));
    addModified(parts);
    addBranch(parts);
    return CSH_CALL_FN_SUCCESS;
}
}
