#include <part/PluginPart.h>
#include "Console.h"

csh::PluginPart::PluginPart(csh::PartConfig &config, const std::wstring &name) : Part(config) {
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
    onUpdateFn = (CShOnUpdateFn) GetProcAddress(hModule, "CShOnUpdate");

    std::map<std::wstring, std::wstring> pconfig;
    onLoadPluginFn && onLoadPluginFn(pconfig);

    if (updateTypeFn)
        updateType = updateTypeFn();
}

csh::PluginPart::~PluginPart() {
    FreeLibrary(hModule);
}

bool csh::PluginPart::update(UpdateType type) {
    auto s = canShowFn && canShowFn();
    if (type <= updateType && s && onUpdateFn) {
        onUpdateFn(parts);
    }
    return s;
}

void csh::PluginPart::printContents() {
    for (auto &part: parts) {
        Console::print(part.str);
    }
}
