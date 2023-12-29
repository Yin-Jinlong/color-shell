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

    class PartConfig {
    public:
        Color foregroundColor = Color(255, 255, 255);
        Color backgroundColor = Color(0, 0, 0);
        std::wstring end;
        bool passBgc;
        int maxWidth;
        EllipsisPosition ellipsis;
    };

    const wchar_t DefaultEnd[2] = L"\ue0b0";
    const bool DefaultPassBgc = false;
    const int DefaultMaxWidth = 24;
    const EllipsisPosition DefaultEllipsis = EllipsisPosition::START;

    class BasePartConfig : virtual public csh::PartConfig {
    public:
        BasePartConfig(
                const Color &foregroundColor,
                const Color &backgroundColor,
                std::wstring end = DefaultEnd,
                bool passBgc = DefaultPassBgc,
                int maxWidth = DefaultMaxWidth,
                EllipsisPosition ellipsis = DefaultEllipsis);

        ~BasePartConfig();
    };

    enum class ShowMode {

        NEVER,
        ALWAYS,
        AUTO

    };
}
