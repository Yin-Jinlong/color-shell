#pragma once

#include <predef.h>
#include "Part.h"
#include "IconPartConfig.h"

namespace csh {

    struct UserPartConfig : IconPartConfig {

    };

    class UserPart : public Part {

    private:
        std::wstring user;
    public:

        UserPartConfig *config;

        explicit UserPart(UserPartConfig &config, std::vector<std::wstring> &contents);

        UserPart(UserPart &other);

        ~UserPart();

        void update() override;

        void printContents() override;
    };
}
