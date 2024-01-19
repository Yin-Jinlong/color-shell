#pragma once

#include <predef.h>
#include <Color.h>

#include "PartConfig.h"

namespace csh {

    struct IconPartConfig : BasePartConfig {
    public:
        str      icon;
        ShowMode iconShowMode = ShowMode::Auto;

        IconPartConfig &operator=(const IconPartConfig &other) = default;

        IconPartConfig &operator=(IconPartConfig &&other) noexcept = default;
    };

}
