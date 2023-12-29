#pragma once

#include <predef.h>
#include <vector>
#include "PartConfig.h"

namespace csh {

    class Part {
    public:
        PartConfig *config;
        std::vector<std::wstring> contents;
        Part *last = nullptr;
        Part *next = nullptr;

        Part(PartConfig &config, std::vector<std::wstring> &contents);

        ~Part();

        USE_RET virtual bool canShow() const;

        virtual void update();

        virtual void printContents();

        void printWithMaxWidth(const std::wstring &text) const;

        void print();

        void printContentString(int i) const;

    };
}
