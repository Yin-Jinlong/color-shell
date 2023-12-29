#include "part/Part.h"
#include "Console.h"

csh::Part::Part(csh::PartConfig &config, std::vector<std::wstring> &contents) {
    this->config = &config;
    this->contents = contents;
}

csh::Part::~Part() = default;

bool csh::Part::canShow() const {
    return true;
}

void csh::Part::update() {

}

void csh::Part::printContents() {

}


void csh::Part::printWithMaxWidth(const std::wstring &text) const {
    if (text.empty())
        return;
    auto len = text.length();
    if (len <= config->maxWidth || config->maxWidth <= 3) {
        std::wcout << text;
        return;
    }
    auto maxWidth = config->maxWidth;
    switch (config->ellipsis) {
        case EllipsisPosition::NONE:
            break;
        case EllipsisPosition::START:
            std::wcout << L"...";
            std::wcout << text.substr(len - maxWidth + 3);
            break;
        case EllipsisPosition::CENTER:
            std::wcout << text.substr(0, maxWidth / 2 - 2);
            std::wcout << L"...";
            std::wcout << text.substr(len - maxWidth / 2 + 1);
            break;
        case EllipsisPosition::END:
            std::wcout << text.substr(0, maxWidth - 3);
            std::wcout << L"...";
            break;
    }
}

void csh::Part::print() {
    Console::setColor(config->foregroundColor, config->backgroundColor);
    printContents();
    std::wcout << L" ";
    Console::reset();
    if (config->passBgc || !next) {
        Console::setForegroundColor(config->backgroundColor);
    } else {
        Console::setColor(config->backgroundColor, next->config->backgroundColor);
    }
    std::wcout << config->end;
}

void csh::Part::printContentString(int i)const{
    if (i < contents.size())
        std::wcout << contents[i];
}
