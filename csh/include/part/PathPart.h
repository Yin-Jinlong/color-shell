#pragma once

#include <predef.h>
#include "Part.h"
#include "IconPartConfig.h"

namespace csh {

    struct PathPartConfig : IconPartConfig {

    };

    class PathPart : public Part {

    private:
        std::wstring path;
    public:

        PathPartConfig *config;

        explicit PathPart(PathPartConfig &config, std::vector<std::wstring> &contents);

        PathPart(PathPart &other);

        ~PathPart();

        void update() override;

        void printContents() override;
    };
}
