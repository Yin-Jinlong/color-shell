#pragma once

#include <predef.h>
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

/**
 * CShConfig
 */
class CShConfig {
private:
    std::vector<YAML::Node> configs;
public:
    explicit CShConfig(const std::vector<str> &configFiles);

    ~CShConfig();

    USE_RET str getPrDefault(const str &key, const str &def) const;

    const str *operator[](const str &key) const;
};
