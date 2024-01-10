#include <part/UserPart.h>
#include <windows.h>
#include <Lmcons.h>
#include "Console.h"


csh::UserPart::UserPart(csh::UserPartConfig &config, std::vector<str> &contents) : Part(config, contents) {
    this->config                  = new UserPartConfig;
    this->config->foregroundColor = config.foregroundColor;
    this->config->backgroundColor = config.backgroundColor;
    this->config->end             = config.end;
    this->config->icon            = config.icon;
    this->config->iconShowMode    = config.iconShowMode;
    this->config->passBgc         = config.passBgc;
    this->config->ellipsis        = config.ellipsis;
}

csh::UserPart::UserPart(csh::UserPart &other) : UserPart(*other.config, other.contents) {

}

csh::UserPart::~UserPart() {
    delete config;
}

bool csh::UserPart::update(UpdateType type) {
    if (type > UpdateType::Init)
        return true;
    char  buf[UNLEN + 1];
    DWORD len = UNLEN + 1;
    GetUserNameA(buf, &len);
    this->user = buf;
    return true;
}

void csh::UserPart::printContents() {
    if (config->iconShowMode != ShowMode::Never) {
        Console::print(config->icon);
    }
    Console::print(user);
    printContentString(0);
}
