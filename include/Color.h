#pragma once

#include <predef.h>

namespace csh {
    /**
     * 颜色
     */
    class DLL_OUT Color {
    public:
        u8 r, g, b;

        constexpr explicit Color(u8 r, u8 g, u8 b) : r(r), g(g), b(b) {};

        constexpr ~Color() = default;

    };

    /**
     * 白色
     */
    static constexpr Color White(255, 255, 255);
    /**
     * 黑色
     */
    static constexpr Color Black(0, 0, 0);
    /**
     * 红色
     */
    static constexpr Color Red(255, 0, 0);
    /**
     * 绿色
     */
    static constexpr Color Green(0, 255, 0);
    /**
     * 蓝色
     */
    static constexpr Color Blue(0, 0, 255);
    /**
     * 黄色
     */
    static constexpr Color Yellow(255, 255, 0);
    /**
     * 品红色
     */
    static constexpr Color Magenta(255, 0, 255);
    /**
     * 青色
     */
    static constexpr Color Cyan(0, 255, 255);
    /**
     * 灰色（中间）
     */
    static constexpr Color Gray(128, 128, 128);

}

