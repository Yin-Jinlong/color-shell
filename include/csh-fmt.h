#pragma once

#include <predef.h>
#include <utility>

// @formatter:off
namespace csh {

    struct StrValue {
    public:
        str value;
    };

    template<class T>
    class DLL_OUT Arg : public StrValue {
    public:
        Arg(T v) : StrValue(v.toString()) {};
    };

    template<>
    class DLL_OUT Arg<const char *> : public StrValue {
    public:
        Arg(const char * s) : StrValue(s) {}
    };

    template<>
    class DLL_OUT Arg<str> : public StrValue {
    public:
        Arg(str s) : StrValue(std::move(s)) {}
    };

    template<>
    class DLL_OUT Arg<bool> : public StrValue {
    public:
        Arg(bool v) : StrValue(v?"true":"false") {}
    };

    template<>
    class DLL_OUT Arg<i8> : public StrValue {
    public:
        Arg(i8 v) : StrValue(std::to_string(v)) {}
    };

    template<>
    class DLL_OUT Arg<u8> : public StrValue {
    public:
        Arg(u8 v) : StrValue(std::to_string(v)) {}
    };

    template<>
    class DLL_OUT Arg<i16> : public StrValue {
    public:
        Arg(i16 v) : StrValue(std::to_string(v)) {}
    };

    template<>
    class DLL_OUT Arg<u16> : public StrValue {
    public:
        Arg(u16 v) : StrValue(std::to_string(v)) {}
    };

    template<>
    class DLL_OUT Arg<i32> : public StrValue {
    public:
        Arg(i32 v) : StrValue(std::to_string(v)) {}
    };

    template<>
    class DLL_OUT Arg<u32> : public StrValue {
    public:
        Arg(u32 v) : StrValue(std::to_string(v)) {}
    };

    template<>
    class DLL_OUT Arg<i64> : public StrValue {
    public:
        Arg(i64 v) : StrValue(std::to_string(v)) {}
    };

    template<>
    class DLL_OUT Arg<u64> : public StrValue {
    public:
        Arg(u64 v) : StrValue(std::to_string(v)) {}
    };

    template<>
    class DLL_OUT Arg<char> : public StrValue {
    public:
        Arg(char v) : StrValue({v,0}) {}
    };

}
//@formatter:on

namespace csh {

    /**
     * 格式化字符串
     *
     * @param fmt 格式化字符串
     * @param args 参数
     * @return 格式化后的字符串
     */
    DLL_OUT str format(const char *fmt, std::initializer_list<StrValue> args);

    /**
     * 格式化字符串
     *
     * @tparam Args 参数类型列表
     * @param fmt 格式化字符串
     * @param args 参数列表
     * @return 格式化后的字符串
     */
    template<typename... Args>
    DLL_OUT inline str format(const char *fmt, Args... args) {
        return csh::format(fmt, {csh::Arg(args)...});
    }

}
