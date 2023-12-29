#include "part/IconPartConfig.h"

#include <utility>


csh::IconPartConfig::IconPartConfig(const csh::Color &foregroundColor, const csh::Color &backgroundColor,
                                    const wchar_t *icon, csh::ShowMode iconShowMode, std::wstring end, bool passBgc,
                                    int maxWidth, csh::EllipsisPosition ellipsis) :
        BasePartConfig(foregroundColor, backgroundColor, std::move(end), passBgc, maxWidth, ellipsis) {
    this->icon = icon;
    this->iconShowMode = iconShowMode;
}

csh::IconPartConfig::~IconPartConfig() = default;
