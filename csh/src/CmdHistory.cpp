#include <fstream>
#include <File.h>
#include <str/string-util.h>

#include "../include/CmdHistory.h"


csh::CmdHistory::CmdHistory() = default;

csh::CmdHistory::~CmdHistory() {

    delete file;

    Item *i = head;
    while (i) {
        Item *next = i->next;
        delete i;
        i = next;
    }
}

bool csh::CmdHistory::add(const str &cmd) {
    if (cmd.empty() || (size >= maxSize && !removeHead())) {
        return false;
    }
    if (size && tail->str == cmd)
        return false;
    auto i = new Item;
    i->str = cmd;
    i->last = tail;
    if (size) {
        tail->next = i;
        tail = i;
    } else {
        head = tail = i;
    }
    size++;
    return true;
}

bool csh::CmdHistory::removeHead() {
    if (size) {
        Item *i = head;
        head = i->next;
        delete i;
        size--;
        if (size == 1) {
            tail = head;
        } else if (size == 0) {
            head = tail = nullptr;
        }
        return true;
    }
    return false;
}

void csh::CmdHistory::reset() {
    now = nullptr;
}

str *csh::CmdHistory::last() {
    if (!size)
        return nullptr;
    if (!now) {
        now = tail;
        return &now->str;
    }
    if (now->last)
        now = now->last;
    return &now->str;
}

str *csh::CmdHistory::next() {
    if (!size)
        return nullptr;
    if (now) {
        now = now->next;
    }
    if (now) {
        return &now->str;
    }
    return nullptr;
}

void csh::CmdHistory::clear() {
    while (removeHead());
}

bool csh::CmdHistory::load(csh::File &f) {
    this->file = new File(f.getPath());
    if (!f.exists())
        return false;
    std::vector<str> lines;
    strSplit(file->readAllTexts(), lines, '\n');
    for (auto &line: lines) {
        line = strTrim(line, true, true);
        if (line.empty())
            continue;
        add(line);
    }
    return true;
}

bool csh::CmdHistory::save() {
    if (!file)
        return false;
    std::ofstream os(file->getPath());
    if (!os.is_open())
        return false;
    for (auto i = head; i; i = i->next) {
        os << i->str << std::endl;
    }
    os.close();
    return true;
}

bool csh::CmdHistory::empty() const {
    return !size;
}

u32 csh::CmdHistory::length() const {
    return size;
}

csh::CmdHistory::Iterator csh::CmdHistory::begin() {
    return csh::CmdHistory::Iterator(tail);
}

bool csh::CmdHistory::operator+=(const str &cmd) {
    return add(cmd);
}
