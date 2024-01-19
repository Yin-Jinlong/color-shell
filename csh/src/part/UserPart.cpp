#include <windows.h>
#include <Lmcons.h>
#include <Console.h>

#include "../../include/part/UserPart.h"

csh::UserPart::UserPart(csh::UserPartConfig &config, std::vector<str> &contents) : Part(config, contents) {
    this->config = new UserPartConfig;
    *this->config = config;
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
