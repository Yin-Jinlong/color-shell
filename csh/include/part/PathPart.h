#pragma once

#include <predef.h>
#include "Part.h"
#include "IconPartConfig.h"

namespace csh {

    struct PathPartConfig : IconPartConfig {

    };

    class PathPart : public Part {

    private:
        wstr path;
    public:

        PathPartConfig *config;

        explicit PathPart(PathPartConfig &config, std::vector<wstr> &contents);

        PathPart(PathPart &other);

        ~PathPart();

        bool update(UpdateType type) override;

        void printContents() override;
    };
}
