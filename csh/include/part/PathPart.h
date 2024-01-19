#pragma once

#include <predef.h>

#include "Part.h"
#include "IconPartConfig.h"

namespace csh {

    struct PathPartConfig : IconPartConfig {

    };

    class PathPart : public Part {

    private:
        str path;
    public:

        PathPartConfig *config;

        explicit PathPart(PathPartConfig &config, std::vector<str> &contents);

        PathPart(PathPart &other);

        ~PathPart();

        bool update(UpdateType type) override;

        void printContents() override;
    };
}
