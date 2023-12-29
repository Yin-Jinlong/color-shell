#include <Color.h>

namespace csh {
    Color::Color(u8 r, u8 g, u8 b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    Color::~Color() = default;
}
