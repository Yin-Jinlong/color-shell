#include <part/Parts.h>
#include "Console.h"

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

csh::Parts &csh::Parts::operator+=(csh::Part *part) {
    parts.push_back(part);
    return *this;
}
