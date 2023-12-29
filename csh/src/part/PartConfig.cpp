#include "part/PartConfig.h"

#include <utility>

csh::BasePartConfig::BasePartConfig(
        const Color &foregroundColor,
        const Color &backgroundColor,
        std::wstring end,
        bool passBgc, int maxWidth,
        csh::EllipsisPosition ellipsis) : csh::PartConfig() {
    this->foregroundColor = foregroundColor;
    this->backgroundColor = backgroundColor;
    this->end = std::move(end);
    this->passBgc = passBgc;
    this->maxWidth = maxWidth;
    this->ellipsis = ellipsis;
}

csh::BasePartConfig::~BasePartConfig() = default;
