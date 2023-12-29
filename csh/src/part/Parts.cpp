#include <part/Parts.h>
#include "Console.h"

csh::Parts::~Parts() {
    for (auto &part: parts) {
        delete part;
    }
}

void csh::Parts::print() const {
    Console::reset();
    Console::setForegroundColor(parts[0]->config->backgroundColor);
    std::wcout << start;
    std::vector<Part *> showList;
    Part *last = nullptr;
    for (auto part: parts) {
        if (part->canShow()) {
            showList.push_back(part);
            if (last) {
                last->next = part;
                part->last = last;
            }
            last = part;
        }
    }
    if (last)
        last->next = nullptr;
    for (auto part: showList) {
        part->print();
    }
}

void csh::Parts::update() {
    for (auto &part: parts) {
        part->update();
    }
}

csh::Parts &csh::Parts::operator+=(csh::UserPart *part) {
    auto p = new UserPart(*part);
    parts.push_back(p);
    return *this;
}

csh::Parts &csh::Parts::operator+=(csh::PathPart *part) {
    auto p = new PathPart(*part);
    parts.push_back(p);
    return *this;
}
