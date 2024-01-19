#include <windows.h>
#include <utility>
#include <functional>
#include <Console.h>
#include <direct.h>

#include "../../include/part/PathPart.h"


csh::PathPart::PathPart(csh::PathPartConfig &config, std::vector<str> &contents) : Part(config, contents) {
    this->config = new PathPartConfig;
    *this->config = config;
}

csh::PathPart::PathPart(csh::PathPart &other) : PathPart(*other.config,
                                                         other.contents) {

}

csh::PathPart::~PathPart() {
    delete config;
}

bool csh::PathPart::update(UpdateType type) {
    if (type > UpdateType::WorkDirChanged)
        return true;
    char buf[MAX_PATH];
    _getcwd(buf, MAX_PATH);
    path = buf;
    return true;
}

void csh::PathPart::printContents() {
    if (config->iconShowMode != ShowMode::Never) {
        Console::print(config->icon);
    }
    printWithMaxWidth(*config, path);
    printContentString(0);
}

