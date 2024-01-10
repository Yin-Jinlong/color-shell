#include <CmdList.h>

csh::CmdList::CmdList() = default;

csh::CmdList::~CmdList() = default;

void csh::CmdList::add(const str &cmd) {
    if (map.find(cmd) == map.end()) {
        map.insert({cmd, 1});
    } else {
        map[cmd]++;
    }
}

void csh::CmdList::addCurrent(const str &cmd) {
    if (curMap.find(cmd) == curMap.end()) {
        curMap.insert({cmd, 1});
    } else {
        curMap[cmd]++;
    }
}

void cmdListFind(std::map<str, int> &map, const str &cmd, std::vector<str> &list) {
    for (auto &e: map) {
        if (e.first.starts_with(cmd)) {
            list.push_back(e.first);
        }
    }
}

void csh::CmdList::match(const str &cmd, std::vector<str> &list) {
    cmdListFind(curMap, cmd, list);
    cmdListFind(map, cmd, list);
}

str csh::CmdList::matchOne(const str &cmd) {
    for (auto &e: curMap) {
        if (e.first.starts_with(cmd)) {
            return e.first;
        }
    }
    for (auto &e: map) {
        if (e.first.starts_with(cmd)) {
            return e.first;
        }
    }
    return "";
}

void csh::CmdList::remove(const str &cmd) {
    if (map.find(cmd) == map.end())
        return;
    if (map[cmd] == 1)
        map.erase(cmd);
    else
        map[cmd]--;
}

bool csh::CmdList::operator[](const str &cmd) {
    return curMap.find(cmd) != curMap.end() || map.find(cmd) != map.end();
}

void csh::CmdList::operator+=(const str &cmd) {
    add(cmd);
}

void csh::CmdList::clearCurrent() {
    curMap.clear();
}

void csh::CmdList::clear() {
    map.clear();
    curMap.clear();
}
