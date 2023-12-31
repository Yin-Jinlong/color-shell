#pragma once

#include <predef.h>
#include <windows.h>
#include <csh-plugin.h>
#include "Part.h"

namespace csh {
    class PluginPart : public Part {

    private:
        HMODULE hModule;
        CShOnLoadPluginFn onLoadPluginFn;
        CShUpdateTypeFn updateTypeFn;
        CShCanShowFn canShowFn;
        CShOnUpdateFn onUpdateFn;

        std::vector<ColorStrPart> parts;

        UpdateType updateType;
    public:
        std::wstring name;

        explicit PluginPart(csh::PartConfig &config,std::wstring &name);

        ~PluginPart();

        bool update(UpdateType type) override;

        void printContents() override;
    };
}

