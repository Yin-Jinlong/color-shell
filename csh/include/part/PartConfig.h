#pragma once

#include "predef.h"
#include "Color.h"

namespace csh {

    enum class EllipsisPosition {
        NONE,
        START,
        CENTER,
        END
    };

    const wchar_t DefaultEnd[2] = L"\ue0b0";
    const bool DefaultPassBgc = false;
    const int DefaultMaxWidth = 24;
    const EllipsisPosition DefaultEllipsis = EllipsisPosition::START;

    struct PartConfig {
    public:
        Color foregroundColor = Color(255, 255, 255);
        Color backgroundColor = Color(0, 0, 0);
        std::wstring end = DefaultEnd;
        bool passBgc = DefaultPassBgc;
    };

    struct BasePartConfig : csh::PartConfig {
    public:
        int maxWidth = DefaultMaxWidth;
        EllipsisPosition ellipsis = DefaultEllipsis;
    };

    enum class ShowMode {

        NEVER,
        ALWAYS,
        AUTO

    };
}
