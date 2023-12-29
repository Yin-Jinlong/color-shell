#pragma once

#include <predef.h>
#include <vector>
#include "Part.h"

namespace csh {
    class Parts {
    private:
        std::vector<csh::Part*> parts;
    public:
        const wchar_t start[2] = L"\ue0b6";

        Parts() = default;

        void print() const;

        void update();

        Parts &operator+=(csh::Part *part);

    };
}
