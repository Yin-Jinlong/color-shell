#pragma once

#include <predef.h>

namespace csh {
    class DLL_OUT Color {
    public:
        u8 r, g, b;

        constexpr explicit Color(u8 r, u8 g, u8 b) : r(r), g(g), b(b) {};

        constexpr ~Color() = default;

    };

    static constexpr Color White(255, 255, 255);
    static constexpr Color Black(0, 0, 0);
    static constexpr Color Red(255, 0, 0);
    static constexpr Color Green(0, 255, 0);
    static constexpr Color Blue(0, 0, 255);
    static constexpr Color Yellow(255, 255, 0);
    static constexpr Color Magenta(255, 0, 255);
    static constexpr Color Cyan(0, 255, 255);
    static constexpr Color Gray(128, 128, 128);

}

