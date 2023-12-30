#pragma once

#include <predef.h>
#include <vector>
#include "Part.h"
#include "UserPart.h"
#include "PathPart.h"
#include "PluginPart.h"

namespace csh {
    class Parts {
    private:
        std::vector<csh::Part *> parts;
    public:
        const wchar_t start[2] = L"\ue0b6";

        Parts() = default;

        ~Parts();

        void print() const;

        void update();

        Parts &operator+=(csh::UserPart *part);

        Parts &operator+=(csh::PathPart *part);

        Parts &operator+=(csh::PluginPart *part);

    };
}
