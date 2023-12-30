#include <part/UserPart.h>
#include <windows.h>
#include <Lmcons.h>
#include "Console.h"


csh::UserPart::UserPart(csh::UserPartConfig &config, std::vector<std::wstring> &contents) : Part(config, contents) {
    this->config = new UserPartConfig;
    this->config->foregroundColor = config.foregroundColor;
    this->config->backgroundColor = config.backgroundColor;
    this->config->end = config.end;
    this->config->icon = config.icon;
    this->config->iconShowMode = config.iconShowMode;
    this->config->passBgc = config.passBgc;
    this->config->ellipsis = config.ellipsis;
}

csh::UserPart::UserPart(csh::UserPart &other) : UserPart(*other.config, other.contents) {

}

csh::UserPart::~UserPart() {
    delete config;
}

void csh::UserPart::update() {
    wchar_t buf[UNLEN + 1];
    DWORD len = UNLEN + 1;
    GetUserNameW(buf, &len);
    this->user = buf;
}

void csh::UserPart::printContents() {
    if (config->iconShowMode != ShowMode::NEVER) {
        Console::print(config->icon);
    }
    Console::print(user);
    printContentString(0);
}
