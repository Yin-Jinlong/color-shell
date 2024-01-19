#include <Console.h>

#include "../../include/part/Parts.h"


csh::Parts::~Parts() {
    for (Part *&part: parts) {
        delete part;
    }
}

void csh::Parts::print() const {
    Console::reset();
    Console::setForegroundColor(parts[0]->config->backgroundColor);
    Console::print(start);
    for (Part *part: showList) {
        part->print();
    }
}

void csh::Parts::update(UpdateType type) {
    Part *last = nullptr;
    showList.clear();
    for (Part *part: parts) {
        if (part->update(type)) {
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

csh::Parts &csh::Parts::operator+=(csh::PluginPart *part) {
    auto p = new PluginPart(*part);
    parts.push_back(p);
    return *this;
}
