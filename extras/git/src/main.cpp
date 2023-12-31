#include <predef.h>
#include <csh-plugin.h>
#include "util.h"
#include "str/wstring-util.h"

extern "C" {
DLL_OUT CallResult CShOnLoadPlugin(std::map<std::wstring, std::wstring> &config) {

    return CSH_CALL_FN_SUCCESS;
}

DLL_OUT csh::UpdateType CShUpdateType() {
    return csh::UpdateType::DIR_MODIFIED;
}

DLL_OUT bool CShCanShow() {
    try {
        auto gitOut = getProcessOutput(L"git rev-parse --is-inside-work-tree");
        return gitOut == "true\n";
    } catch (...) {
    }
    return false;
}

void addModified(std::vector<csh::ColorStrPart> &parts) {
    auto modified = getProcessOutput(L"git diff --shortstat");
    modified.erase(modified.begin());
    auto i = modified.find_first_of(' ');
    if (i == std::string::npos)
        return;
    auto s = str_to_wstr(CP_UTF8, modified.substr(0, i));
    parts.push_back(csh::ColorStrPart(std::format(L"\uF4D2 *{} ", s), csh::White));
}

void addBranch(std::vector<csh::ColorStrPart> &parts) {
    auto branch = getProcessOutput(L"git rev-parse --abbrev-ref HEAD");
    branch.erase(branch.end() - 1);
    if (branch.length() > 32)
        return;
    auto b = str_to_wstr(CP_UTF8, branch);
    parts.push_back(csh::ColorStrPart(std::format(L"\U000f062c {}", std::wstring(b)), csh::White));
}

DLL_OUT CallResult CShOnUpdate(std::vector<csh::ColorStrPart> &parts) {
    parts.clear();
    parts.push_back(csh::ColorStrPart(L" \U000f02a2 ", csh::White));
    addModified(parts);
    addBranch(parts);
    return CSH_CALL_FN_SUCCESS;
}
}
