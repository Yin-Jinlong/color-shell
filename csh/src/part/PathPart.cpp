#pragma once

#include <part/PathPart.h>
#include <windows.h>

#include <utility>
#include <functional>

csh::PathPart::PathPart(csh::PathPartConfig &config, std::vector<std::wstring> &contents) : Part(config, contents) {
    this->config = &config;
}

void csh::PathPart::update() {
    wchar_t buf[MAX_PATH];
    _wgetcwd(buf, MAX_PATH);
    path = buf;
}

void csh::PathPart::printContents(){
    if (config->iconShowMode != ShowMode::NEVER) {
        std::wcout << this->config->icon;
    }
    printWithMaxWidth(path);
    printContentString(0);
}

csh::PathPartConfig::PathPartConfig(const csh::Color &foregroundColor, const csh::Color &backgroundColor,
                                    const wchar_t *icon, csh::ShowMode showMode, std::wstring end, bool passBgc,
                                    int maxWidth, csh::EllipsisPosition ellipsis) :
        IconPartConfig(foregroundColor,
                       backgroundColor,
                       icon, showMode, std::move(end),
                       passBgc,
                       maxWidth,
                       ellipsis),
        BasePartConfig(foregroundColor,
                       backgroundColor,
                       end,
                       passBgc,
                       maxWidth,
                       ellipsis) {

}

csh::PathPartConfig::~PathPartConfig() = default;
