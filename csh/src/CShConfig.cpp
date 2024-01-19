#include <File.h>
#include <str/string-util.h>

#include "../include/CShConfig.h"


CShConfig::CShConfig(const std::vector<str> &configFiles) {
    for (auto &f: configFiles) {
        if (f.empty()) continue;
        try {
            configs.push_back(YAML::Load(csh::File(f).readAllTexts()));
        } catch (...) {

        }
    }
}

const str *findValue(const YAML::Node &node, const std::vector<str> &keys, int i) {
    if (i == keys.size()) {
        if (node.IsScalar())
            return &node.Scalar();
        return nullptr;
    }
    return findValue(node[keys[i]], keys, i + 1);
}

str CShConfig::getPrDefault(const str &key, const str &def) const {
    const str *v = operator[](key);
    return v ? *v : def;
}

const str *CShConfig::operator[](const str &key) const {
    std::vector<str> keys;
    strSplit(key, keys, '.');
    for (auto &c: configs) {
        const str *s = findValue(c, keys, 0);
        if (s)
            return s;
    }
    return nullptr;
}

CShConfig::~CShConfig() = default;
