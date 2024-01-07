#include "part/Part.h"
#include "Console.h"


csh::Part::Part(csh::PartConfig &config) {
    this->config                  = new PartConfig;
    this->config->foregroundColor = config.foregroundColor;
    this->config->backgroundColor = config.backgroundColor;
    this->config->end             = config.end;
    this->config->passBgc         = config.passBgc;
}

csh::Part::Part(csh::PartConfig &config, std::vector<wstr> &contents) {
    this->config                  = new PartConfig;
    this->config->foregroundColor = config.foregroundColor;
    this->config->backgroundColor = config.backgroundColor;
    this->config->end             = config.end;
    this->config->passBgc         = config.passBgc;
    this->contents                = contents;
}

csh::Part::~Part() {
    delete config;
}

bool csh::Part::update(UpdateType type) {
    return true;
}

void csh::Part::printContents() {

}


void csh::Part::printWithMaxWidth(const BasePartConfig &config, const wstr &text) {
    if (text.empty())
        return;
    size_t len = text.length();
    if (len <= config.maxWidth || config.maxWidth <= 3) {
        Console::print(text);
        return;
    }
    int maxWidth = config.maxWidth;
    switch (config.ellipsis) {
        case EllipsisPosition::None:
            break;
        case EllipsisPosition::Start:
            Console::print(L"...");
            Console::print(text.substr(len - maxWidth + 3));
            break;
        case EllipsisPosition::Center:
            Console::print(text.substr(0, maxWidth / 2 - 2));
            Console::print(L"...");
            Console::print(text.substr(len - maxWidth / 2 + 1));
            break;
        case EllipsisPosition::End:
            Console::print(text.substr(0, maxWidth / 2 - 2));
            Console::print(L"...");
            break;
    }
}

void csh::Part::print() {
    Console::setColor(config->foregroundColor, config->backgroundColor);
    printContents();
    Console::print(L" ");
    Console::reset();
    if (config->passBgc || !next) {
        Console::setForegroundColor(config->backgroundColor);
    } else {
        Console::setColor(config->backgroundColor, next->config->backgroundColor);
    }
    Console::print(config->end);
}

void csh::Part::printContentString(int i) const {
    if (i < contents.size())
        Console::print(contents[i]);
}
