#pragma once

#include <predef.h>
#include <File.h>

namespace csh {

    /**
     * 命令历史
     */
    class CmdHistory {
    private:
        struct Item {
            Item *last = nullptr;
            Item *next = nullptr;
            str  str;
        };

        /**
         * 最大长度
         */
        u32  maxSize = INT_MAX;
        /**
         * 长度
         */
        u32  size    = 0;
        /**
         * 头
         */
        Item *head   = nullptr;
        /**
         * 尾
         */
        Item *tail   = nullptr;
        /**
         * 当前
         */
        Item *now    = nullptr;

        File *file = nullptr;
    public:

        class Iterator {
        private:
            Item *item = nullptr;
        public:
            constexpr explicit Iterator(Item *item) : item(item) {}

            constexpr str &operator*() const {
                return item->str;
            }

            constexpr Iterator operator++() {
                item = item ? item->last : nullptr;
                return *this;
            }

            constexpr bool operator==(const Iterator &other) const {
                return item == other.item;
            }

            constexpr bool operator!=(const Iterator &other) const {
                return item != other.item;
            }
        };

        CmdHistory();

        ~CmdHistory();

        /**
         * 添加
         *
         * @param cmd 命令
         * @return 是否成功
         */
        bool add(const str &cmd);

        /**
         * 移除头部
         *
         * @return 是否成功
         */
        bool removeHead();

        /**
         * 重置（当前）
         */
        void reset();

        /**
         * 上一个命令
         *
         * @return 命令或第一个或空
         */
        str *last();

        /**
         * 下一个命令
         *
         * @return 命令或空
         */
        str *next();

        /**
         * 清空
         */
        void clear();

        /**
         * 加载
         *
         * @param file 文件
         */
        bool load(csh::File &f);

        /**
         * 保存
         *
         * @param file 文件
         */
        bool save();

        /**
         * 是否为空
         *
         * @return 是否为空
         */
        USE_RET bool empty() const;

        /**
         * 长度
         *
         * @return 长度
         */
        USE_RET u32 length() const;

        /**
         * 开头，列表尾部
         */
        Iterator begin();

        /**
         * 结尾，列表头部
         */
        static constexpr Iterator end() {
            return Iterator(nullptr);
        }

        /**
         * 追加
         *
         * @param cmd 命令
         * @return 是否成功
         */
        bool operator+=(const str &cmd);
    };
}
