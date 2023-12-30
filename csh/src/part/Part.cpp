#include "part/Part.h"
#include "Console.h"


csh::Part::Part(csh::PartConfig &config) {
    this->config = new PartConfig;
    this->config->foregroundColor = config.foregroundColor;
    this->config->backgroundColor = config.backgroundColor;
    this->config->end = config.end;
    this->config->passBgc = config.passBgc;
}

csh::Part::Part(csh::PartConfig &config, std::vector<std::wstring> &contents) {
    this->config = new PartConfig;
    this->config->foregroundColor = config.foregroundColor;
    this->config->backgroundColor = config.backgroundColor;
    this->config->end = config.end;
    this->config->passBgc = config.passBgc;
    this->contents = contents;
}

csh::Part::~Part() {
    delete config;
}

bool csh::Part::canShow() const {
    return true;
}

void csh::Part::update() {

}

void csh::Part::printContents() {

}


void csh::Part::printWithMaxWidth(const BasePartConfig &config, const std::wstring &text) {
    if (text.empty())
        return;
    auto len = text.length();
    if (len <= config.maxWidth || config.maxWidth <= 3) {
        Console::print(text);
        return;
    }
    auto maxWidth = config.maxWidth;
    switch (config.ellipsis) {
        case EllipsisPosition::NONE:
            break;
        case EllipsisPosition::START:
            Console::print("...");
            Console::print(text.substr(len - maxWidth + 3));
            break;
        case EllipsisPosition::CENTER:
            Console::print(text.substr(0, maxWidth / 2 - 2));
            Console::print("...");
            Console::print(text.substr(len - maxWidth / 2 + 1));
            break;
        case EllipsisPosition::END:
            Console::print(text.substr(0, maxWidth / 2 - 2));
            Console::print("...");
            break;
    }
}

void csh::Part::print() {
    Console::setColor(config->foregroundColor, config->backgroundColor);
    printContents();
    Console::print(" ");
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
