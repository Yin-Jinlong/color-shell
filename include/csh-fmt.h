#pragma once

#include <predef.h>

#include <utility>

namespace csh {

    class DLL_OUT Arg {
    public:
        str value;

        Arg(char c) : value({c, 0}) {};

        Arg(int i) : value(std::to_string(i)) {};

        Arg(unsigned int i) : value(std::to_string(i)) {}

        Arg(long l) : value(std::to_string(l)) {};

        Arg(unsigned long l) : value(std::to_string(l)) {};

        Arg(float f) : value(std::to_string(f)) {};

        Arg(double d) : value(std::to_string(d)) {};

        Arg(str s) : value(std::move(s)) {};

        Arg(const char *s) : value(s) {};

    };

    /**
     * 格式化字符串
     *
     * @param fmt 格式化字符串
     * @param args 参数
     * @return 格式化后的字符串
     */
    DLL_OUT str format(const char *fmt, std::initializer_list<Arg> args);
}
