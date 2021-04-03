#ifndef COLOR_H
#define COLOR_H

#include <cstdlib>
#include <iostream>
#include <algorithm>

struct Color {
    static const uint8_t MAX_COLOR_VALUE = 255u;
    uint8_t red_{0};
    uint8_t green_{0};
    uint8_t blue_{0};

    Color() = default;

    Color(uint32_t R, uint32_t G, uint32_t B) {
        red_ = std::clamp(R, 0u, static_cast<uint32_t>(MAX_COLOR_VALUE));
        green_ = std::clamp(G, 0u, static_cast<uint32_t>(MAX_COLOR_VALUE));
        blue_ = std::clamp(B, 0u, static_cast<uint32_t>(MAX_COLOR_VALUE));
    }

};

inline std::ostream& operator<<(std::ostream& out, uint8_t val) {
    return out << static_cast<uint32_t>(val);
}

inline std::ostream& operator << (std::ostream& out, const Color& c) {
    return out << c.red_ << ' ' << c.green_ << ' ' << c.blue_;
}

#endif
