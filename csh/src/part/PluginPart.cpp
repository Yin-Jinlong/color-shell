#include <part/PluginPart.h>
#include "Console.h"

csh::PluginPart::PluginPart(csh::PartConfig &config, std::wstring &name) : Part(config) {
    this->name = name;

    std::wstring path = L"../extras/";
    path += name;
    path += L"/csh-";
    path += name;
    path += L".dll";

    hModule = LoadLibraryW(path.c_str());
    if (!hModule) {
        throw std::runtime_error("Failed to load plugin ");
    }
    onLoadPluginFn = (CShOnLoadPluginFn) GetProcAddress(hModule, "CShOnLoadPlugin");
    canShowFn = (CShCanShowFn) GetProcAddress(hModule, "CShCanShow");
    updateTypeFn = (CShUpdateTypeFn) GetProcAddress(hModule, "CShUpdateType");
    onUpdatePlugin = (CShOnUpdateFn) GetProcAddress(hModule, "CShOnUpdate");

    std::map<std::wstring, std::wstring> pconfig;
    onLoadPluginFn && onLoadPluginFn(pconfig);

    if (updateTypeFn)
        updateType = updateTypeFn();
}

csh::PluginPart::~PluginPart() {
    FreeLibrary(hModule);
}

bool csh::PluginPart::update(UpdateType type) {
    if (updateType <= type) {
        onUpdatePlugin && onUpdatePlugin(parts);
        return canShowFn && canShowFn();
    }
    return true;
}

void csh::PluginPart::printContents() {
    for (auto &part: parts) {
        Console::print(part.str);
    }
}
