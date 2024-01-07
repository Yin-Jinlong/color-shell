#include <str/char-util.h>

bool is_high_surrogate(wchar_t wc) {
    return wc >= 0xD800 && wc <= 0xDBFF;
}

bool is_low_surrogate(wchar_t wc) {
    return wc >= 0xDC00 && wc <= 0xDFFF;
}

#define AND(a, b) (c>=(a) && c<=(b))

bool is_full_width_char(wchar_t c) {
    // 不完全符合，还可能有预留和空白
    return  // 韩文字母 https://www.unicode.org/charts/PDF/U1100.pdf
            AND(0x1100, 0x11FF) ||
            // 杂项 https://www.unicode.org/charts/PDF/U2600.pdf
            // 装饰 https://www.unicode.org/charts/PDF/U2700.pdf
            AND(0x2600, 0x27BF) ||
            // 盲文符号 https://www.unicode.org/charts/PDF/U2800.pdf
            AND(0x2800, 0x28FF) ||
            // CJK 偏旁部首 https://www.unicode.org/charts/PDF/U2E80.pdf
            // 康熙部首     https://www.unicode.org/charts/PDF/U2F00.pdf
            AND(0x2E80, 0x2FDF) ||
            // 汉字结构描述 https://www.unicode.org/charts/PDF/U2FF0.pdf
            // CJK 标点   http://www.unicode.org/charts/PDF/U3000.pdf
            // 平假       https://www.unicode.org/charts/PDF/U3040.pdf
            // 片假       https://www.unicode.org/charts/PDF/U30A0.pdf
            // 汉字注音符号 https://www.unicode.org/charts/PDF/U3100.pdf
            // 韩文兼容字母 https://www.unicode.org/charts/PDF/U3130.pdf
            AND(0x2FF0, 0x318E) ||
            // 汉字注音符号扩展 https://www.unicode.org/charts/PDF/U31A0.pdf
            // CJK 笔画      http://www.unicode.org/charts/PDF/U31C0.pdf
            // 日文片假扩展    https://www.unicode.org/charts/PDF/U31F0.pdf
            // CJK 字母月份   https://www.unicode.org/charts/PDF/U3200.pdf
            // 特殊符号       https://www.unicode.org/charts/PDF/U3300.pdf
            AND(0x31A0, 0x33FF) ||
            // 易经符号 https://www.unicode.org/charts/PDF/U4DC0.pdf
            // CJK https://www.unicode.org/charts/PDF/U4E00.pdf
            AND(0x4DC0, 0x9FFF) ||
            // 彝文音节 https://www.unicode.org/charts/PDF/UA000.pdf
            // 彝文部首 https://www.unicode.org/charts/PDF/UA490.pdf
            AND(0xA000, 0xA4C6) ||
            // 韩文拼音 https://www.unicode.org/charts/PDF/UAC00.pdf
            AND(0xAC00, 0xD7AF) ||
            // 中文竖版标点 https://www.unicode.org/charts/PDF/UFE10.pdf
            AND(0xFE10, 0xFE19) ||
            // CJK 兼容符号 https://www.unicode.org/charts/PDF/UFE30.pdf
            AND(0xFE30, 0xFE4F) ||
            // 半角的全角版本 https://www.unicode.org/charts/PDF/UFF00.pdf
            AND(0xFF01, 0xFF5E);
    // 太玄经符号 https://www.unicode.org/charts/PDF/U1D300.pdf
    // AND(0x1D300, 0x1D356);
}

#undef AND
