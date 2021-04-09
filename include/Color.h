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

    Color(int32_t R, int32_t G, int32_t B) {
        red_ = ColorClamp(R);
        green_ = ColorClamp(G);
        blue_ = ColorClamp(B);
    }

    uint8_t ColorClamp(int32_t val) {
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

inline bool operator==(const Color& lhs, const Color& rhs) {
    return lhs.red_==rhs.red_
            && lhs.blue_==rhs.blue_
            && lhs.green_==rhs.green_;
}

#endif
