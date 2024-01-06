#pragma once

#include <predef.h>
#include "Color.h"

/**
 * 控制台
 *
 * <br>
 *
 * 程序打印应尽量使用本类
 *
 * @note 为方便没加命名空间
 */
class DLL_OUT Console {
private:
    /**
     * 是否打开颜色
     */
    static bool withColor;
public:

    /**
     * 设置颜色模式
     *
     * @param on 是否打开
     */
    static void setColorMode(bool on);

    /**
     * 重置颜色
     *
     * @param all 是否全部重置，全部重置相当于clear
     */
    static void reset(bool all = false);

    /**
     * 设置背景颜色
     *
     * @param color 颜色
     */
    static void setBackgroundColor(csh::Color color) {
        setBackgroundColor(color.r, color.g, color.b);
    }

    /**
     * 设置背景颜色
     *
     * @param r 红色
     * @param g 绿色
     * @param b 蓝色
     */
    static void setBackgroundColor(u8 r, u8 g, u8 b);

    /**
     * 设置前景颜色
     *
     * @param color 颜色
     */
    static void setForegroundColor(csh::Color color) {
        setForegroundColor(color.r, color.g, color.b);
    }

    /**
     * 设置前景颜色
     *
     * @param r 红色
     * @param g 绿色
     * @param b 蓝色
     */
    static void setForegroundColor(u8 r, u8 g, u8 b);

    /**
     * 设置颜色
     *
     * @param fc 前景颜色
     * @param bc 背景颜色
     */
    static void setColor(csh::Color &fc, csh::Color &bc) {
        setColor(fc.r, fc.g, fc.b, bc.r, bc.g, bc.b);
    }

    /**
     * 设置颜色
     *
     * @param fr 前景颜色R
     * @param fg 前景颜色G
     * @param fb 前景颜色B
     * @param br 背景色R
     * @param bg 背景色G
     * @param bb 背景色B
     *
     * @note 便捷高效
     */
    static void setColor(u8 fr, u8 fg, u8 fb, u8 br, u8 bg, u8 bb);

    /**
     * 向上移动光标
     *
     * @param n 移动的行数
     */
    static void moveCursorUp(int n = 1);

    /**
     * 向下移动光标
     *
     * @param n 移动的行数
     */
    static void moveCursorDown(int n = 1);

    /**
     * 向左移动光标
     *
     * @param n 移动的行数
     */
    static void moveCursorLeft(int n = 1);

    /**
     * 向右移动光标
     *
     * @param n 移动的行数
     */
    static void moveCursorRight(int n = 1);

    /**
     * 光标到可视末尾
     */
    const static int CLEAR_CURSOR_TO_END   = 0;
    /**
     * 光标到可视开头
     */
    const static int CLEAR_CURSOR_TO_START = 1;
    /**
     * 清除可视区域
     */
    const static int CLEAR_VIEW_ALL        = 2;
    /**
     * 清除所有
     */
    const static int CLEAR_ALL             = 3;

    /**
     * 清除屏幕
     *
     * @param flag 清除的类型
     */
    static void clear(int flag = CLEAR_CURSOR_TO_END);

    /**
     * 打印字符串
     *
     * @param str 字符串
     */
    static void print(const std::wstring &str);

    /**
     * 打印字符串
     *
     * @param str 字符串
     */
    static void print(const wchar_t *str);

    /**
     * 打印字符
     *
     * @param c 字符
     */
    static void print(wchar_t c);

    /**
     * 打印数字
     *
     * @param i 数字
     */
    static void printNum(int i);


};


