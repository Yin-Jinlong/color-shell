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
    updateTimeFn = (CShUpdateTimeFn) GetProcAddress(hModule, "CShUpdateTime");
    onUpdatePlugin = (CShOnUpdateFn) GetProcAddress(hModule, "CShOnUpdate");
    std::map<std::wstring, std::wstring> pconfig;
    onLoadPluginFn && onLoadPluginFn(pconfig);
}

csh::PluginPart::~PluginPart() {
    FreeLibrary(hModule);
}

bool csh::PluginPart::canShow() const {
    return canShowFn && canShowFn();
}

void csh::PluginPart::update() {
    onUpdatePlugin && onUpdatePlugin(parts);
}

void csh::PluginPart::printContents() {
    for (auto &part: parts) {
        Console::print(part.str);
    }
}
