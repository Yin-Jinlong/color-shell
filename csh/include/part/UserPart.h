#pragma once

#include <predef.h>

#include "Part.h"
#include "IconPartConfig.h"

namespace csh {

    struct UserPartConfig : IconPartConfig {

    };

    class UserPart : public Part {

    private:
        str user;
    public:

        UserPartConfig *config;

        explicit UserPart(UserPartConfig &config, std::vector<str> &contents);

        UserPart(UserPart &other);

        ~UserPart();

        bool update(UpdateType type) override;

        void printContents() override;
    };
}
