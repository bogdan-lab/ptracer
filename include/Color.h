#ifndef COLOR_H
#define COLOR_H

#include <cstdlib>
#include <iostream>
#include <limits>

struct Color {
    uint8_t red_{0};
    uint8_t green_{0};
    uint8_t blue_{0};

    Color() = default;

    Color(uint32_t R, uint32_t G, uint32_t B) {
        red_ = ColorClamp(R);
        green_ = ColorClamp(G);
        blue_ = ColorClamp(B);
    }

    uint8_t ColorClamp(uint32_t val) {
        auto high = std::numeric_limits<uint8_t>::max();
        auto low = std::numeric_limits<uint8_t>::min();
        return static_cast<uint8_t>(val>=high ? high : (val<=low ? low : val));
    }

};

inline std::ostream& operator<<(std::ostream& out, uint8_t val) {
    return out << static_cast<uint32_t>(val);
}

inline std::ostream& operator << (std::ostream& out, const Color& c) {
    return out << c.red_ << ' ' << c.green_ << ' ' << c.blue_;
}

#endif
