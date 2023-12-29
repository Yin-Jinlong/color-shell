#pragma once

#include <predef.h>
#include "Part.h"
#include "IconPartConfig.h"

namespace csh {

    class PathPartConfig : virtual public IconPartConfig {
    public:
        PathPartConfig(
                const Color &foregroundColor,
                const Color &backgroundColor,
                const wchar_t *icon,
                ShowMode showMode = ShowMode::AUTO,
                std::wstring end = DefaultEnd,
                bool passBgc = DefaultPassBgc,
                int maxWidth = DefaultMaxWidth,
                EllipsisPosition ellipsis = DefaultEllipsis);

        ~PathPartConfig();
    };

    class PathPart : public Part {

    private:
        std::wstring path;
    public:

        PathPartConfig *config;

        explicit PathPart(PathPartConfig &config, std::vector<std::wstring> &contents);

        void update() override;

        void printContents() override;
    };
}
