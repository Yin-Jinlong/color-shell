#pragma once

#include "predef.h"
#include "Color.h"
#include "PartConfig.h"

namespace csh {

    struct IconPartConfig : BasePartConfig {
    public:
        wstr     icon;
        ShowMode iconShowMode = ShowMode::Auto;
    };

}
