#pragma once

#include "predef.h"
#include "Color.h"
#include "PartConfig.h"

namespace csh {

    class IconPartConfig : virtual public BasePartConfig {
    public:
        const wchar_t *icon;
        ShowMode iconShowMode;

        IconPartConfig(
                const Color &foregroundColor,
                const Color &backgroundColor,
                const wchar_t *icon,
                ShowMode iconShowMode,
                std::wstring end = DefaultEnd,
                bool passBgc = DefaultPassBgc,
                int maxWidth = DefaultMaxWidth,
                EllipsisPosition ellipsis = DefaultEllipsis);

        ~IconPartConfig();
    };

}
