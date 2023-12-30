#include <predef.h>
#include <csh-plugin.h>

extern "C" {
DLL_OUT CallResult CShOnLoadPlugin(std::map<std::wstring, std::wstring> &config) {

    return CSH_CALL_FN_SUCCESS;
}

DLL_OUT csh::UpdateType CShUpdateTime() {
    return csh::UpdateType::INIT;
}

DLL_OUT bool CShCanShow() {
    return true;
}

DLL_OUT CallResult CShOnUpdate(std::vector<csh::ColorStrPart> &parts) {
    parts.clear();
    parts.push_back(csh::ColorStrPart(L"\U000f02a2", csh::Color(0, 0, 0)));
    return CSH_CALL_FN_SUCCESS;
}
}
