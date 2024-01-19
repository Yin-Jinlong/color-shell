#pragma once

#include <predef.h>

#include "Part.h"
#include "UserPart.h"
#include "PathPart.h"
#include "PluginPart.h"

namespace csh {
    class Parts {
    private:
        std::vector<csh::Part *> parts;
        std::vector<Part *>      showList;
    public:
        const char start[4] = "\ue0b6";

        u32 lastModifiedTime = 0;

        bool cd = false;

        Parts() = default;

        ~Parts();

        void print() const;

        void update(UpdateType type);

        Parts &operator+=(csh::UserPart *part);

        Parts &operator+=(csh::PathPart *part);

        Parts &operator+=(csh::PluginPart *part);

    };
}
