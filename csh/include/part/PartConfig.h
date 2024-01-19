#pragma once

#include <predef.h>
#include <Color.h>

namespace csh {

    enum class EllipsisPosition {
        None,
        Start,
        Center,
        End
    };

    const char             DefaultEnd[4]   = "\ue0b0";
    const bool             DefaultPassBgc  = false;
    const int              DefaultMaxWidth = 24;
    const EllipsisPosition DefaultEllipsis = EllipsisPosition::Start;

    struct PartConfig {
    public:
        Color foregroundColor = Color(255, 255, 255);
        Color backgroundColor = Color(0, 0, 0);
        str   end             = DefaultEnd;
        bool  passBgc         = DefaultPassBgc;

        PartConfig &operator=(const PartConfig &other) noexcept = default;

        PartConfig &operator=(PartConfig &&other) noexcept = default;
    };

    struct BasePartConfig : csh::PartConfig {
    public:
        int              maxWidth = DefaultMaxWidth;
        EllipsisPosition ellipsis = DefaultEllipsis;

        BasePartConfig &operator=(const BasePartConfig &other) noexcept = default;

        BasePartConfig &operator=(BasePartConfig &&other) noexcept = default;
    };

    enum class ShowMode {

        Never,
        Always,
        Auto

    };
}
