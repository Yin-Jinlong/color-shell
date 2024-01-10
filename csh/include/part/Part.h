#pragma once

#include <predef.h>
#include <vector>
#include "PartConfig.h"
#include "csh-plugin.h"

namespace csh {

    class Part {
    public:
        PartConfig       *config{};
        std::vector<str> contents;

        Part *last = nullptr;
        Part *next = nullptr;

        explicit Part(PartConfig &config, std::vector<str> &contents);

        explicit Part(PartConfig &config);

        ~Part();

        /**
         * 更新
         * @param type 类型
         * @return 是否可以显示
         */
        virtual bool update(UpdateType type);

        /**
         * 打印内容
         */
        virtual void printContents();

        /**
         * 打印限宽内容
         * @param config 配置
         * @param text 文本
         */
        static void printWithMaxWidth(const BasePartConfig &config, const str &text);

        /**
         * 打印
         */
        void print();

        /**
         * 打印内容字符串
         * @param i 索引
         */
        void printContentString(int i) const;

    };
}
