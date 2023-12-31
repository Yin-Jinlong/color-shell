#pragma once

#include <predef.h>
#include <vector>
#include "PartConfig.h"
#include "csh-plugin.h"

namespace csh {

    class Part {
    public:
        PartConfig *config{};
        std::vector<std::wstring> contents;
        Part *last = nullptr;
        Part *next = nullptr;

        explicit Part(PartConfig &config, std::vector<std::wstring> &contents);

        explicit Part(PartConfig &config);

        ~Part();

        virtual bool update(UpdateType type);

        virtual void printContents();

        static void printWithMaxWidth(const BasePartConfig &config,const std::wstring &text) ;

        void print();

        void printContentString(int i) const;

    };
}