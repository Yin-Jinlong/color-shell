#pragma once

#include <predef.h>

namespace csh{
    class LIB_API Color {
    public:
        u8 r, g, b;

        explicit Color(u8 r, u8 g, u8 b);

        ~Color();
    };
}

