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
    MAYBE_UNUSED static constexpr Color White(255, 255, 255);
    /**
     * 黑色
     */
    MAYBE_UNUSED static constexpr Color Black(0, 0, 0);
    /**
     * 浅红色
     */
    MAYBE_UNUSED static constexpr Color LightRed(255, 64, 64);
    /**
     * 红色
     */
    MAYBE_UNUSED static constexpr Color Red(255, 0, 0);
    /**
     * 浅绿色
     */
    MAYBE_UNUSED static constexpr Color LightGreen(64, 255, 64);
    /**
     * 绿色
     */
    MAYBE_UNUSED static constexpr Color Green(0, 255, 0);
    /**
     * 蓝色
     */
    MAYBE_UNUSED static constexpr Color Blue(0, 0, 255);
    /**
     * 黄色
     */
    MAYBE_UNUSED static constexpr Color Yellow(255, 255, 0);
    /**
     * 品红色
     */
    MAYBE_UNUSED static constexpr Color Magenta(255, 0, 255);
    /**
     * 青色
     */
    MAYBE_UNUSED static constexpr Color Cyan(0, 255, 255);
    /**
     * 浅灰色
     */
    MAYBE_UNUSED static constexpr Color LightGray(192, 192, 192);
    /**
     * 灰色（中间）
     */
    MAYBE_UNUSED static constexpr Color Gray(128, 128, 128);
    /**
     * 暗灰色
     */
    MAYBE_UNUSED static constexpr Color DarkGray(64, 64, 64);

}

