#pragma once

#include <part/PathPart.h>
#include <windows.h>

#include <utility>
#include <functional>
#include <Console.h>

csh::PathPart::PathPart(csh::PathPartConfig &config, std::vector<std::wstring> &contents) : Part(config, contents) {
    this->config = new PathPartConfig;
    this->config->foregroundColor = config.foregroundColor;
    this->config->backgroundColor = config.backgroundColor;
    this->config->icon = config.icon;
    this->config->iconShowMode = config.iconShowMode;
    this->config->end = config.end;
    this->config->passBgc = config.passBgc;
    this->config->maxWidth = config.maxWidth;
    this->config->ellipsis = config.ellipsis;
}

csh::PathPart::PathPart(csh::PathPart &other) : PathPart(*other.config,
                                                         other.contents) {

}

csh::PathPart::~PathPart() {
    delete config;
}

void csh::PathPart::update(UpdateType type) {
    if (type > UpdateType::WORK_DIR_CHANGED)
        return;
    wchar_t buf[MAX_PATH];
    _wgetcwd(buf, MAX_PATH);
    path = buf;
}

void csh::PathPart::printContents() {
    if (config->iconShowMode != ShowMode::NEVER) {
        Console::print(config->icon);
    }
    printWithMaxWidth(*config, path);
    printContentString(0);
}

