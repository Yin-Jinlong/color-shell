#include <part/PluginPart.h>
#include "Console.h"

csh::PluginPart::PluginPart(csh::PartConfig &config, const str &name) : Part(config) {
    this->name = name;

    str path = "../extras/";
    path += name;
    path += "/csh-";
    path += name;
    path += ".dll";

    hModule = LoadLibraryA(path.c_str());
    if (!hModule) {
        throw std::runtime_error("Failed to load plugin ");
    }
    onLoadPluginFn = (CShOnLoadPluginFn) GetProcAddress(hModule, "CShOnLoadPlugin");
    canShowFn      = (CShCanShowFn) GetProcAddress(hModule, "CShCanShow");
    updateTypeFn   = (CShUpdateTypeFn) GetProcAddress(hModule, "CShUpdateType");
    onUpdateFn     = (CShOnUpdateFn) GetProcAddress(hModule, "CShOnUpdate");

    std::map<str, str> cfg;
    onLoadPluginFn && onLoadPluginFn(cfg);

    if (updateTypeFn)
        updateType = updateTypeFn();
}

csh::PluginPart::~PluginPart() {
    FreeLibrary(hModule);
}

bool csh::PluginPart::update(UpdateType type) {
    bool s = canShowFn && canShowFn();
    if (type <= updateType && s && onUpdateFn) {
        onUpdateFn(parts);
    }
    return s;
}

void csh::PluginPart::printContents() {
    for (const ColorStrPartStruct &part: parts) {
        Console::print(part.str);
    }
}
