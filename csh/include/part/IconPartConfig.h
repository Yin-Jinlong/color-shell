#pragma once

#include "predef.h"
#include "Color.h"
#include "PartConfig.h"

namespace csh {

    struct IconPartConfig : BasePartConfig {
    public:
        const wchar_t *icon = L"";
        ShowMode iconShowMode = ShowMode::AUTO;
    };

}
