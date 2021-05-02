#ifndef COLOR_H
#define COLOR_H

#include <cstdlib>
#include <iostream>
#include <limits>

struct Color {
  uint8_t red_ = 0;
  uint8_t green_ = 0;
  uint8_t blue_ = 0;
  uint8_t is_color_ = 0;  // 1 - color, 0 - no_color

  Color() = default;

  Color(int64_t R, int64_t G, int64_t B) : is_color_(1) {
    red_ = ColorClamp(R);
    green_ = ColorClamp(G);
    blue_ = ColorClamp(B);
  }

  uint8_t ColorClamp(int64_t val) {
    auto high = std::numeric_limits<uint8_t>::max();
    auto low = std::numeric_limits<uint8_t>::min();
    return static_cast<uint8_t>(val >= high ? high : (val <= low ? low : val));
  }

  void TruncByColor(const Color& tr_col) {
    if (!IsColor()) return;
    if (red_ > tr_col.red_) red_ = tr_col.red_;
    if (green_ > tr_col.green_) green_ = tr_col.green_;
    if (blue_ > tr_col.blue_) blue_ = tr_col.blue_;
  }

  bool IsColor() const { return is_color_ == 1; }
};

inline std::ostream& operator<<(std::ostream& out, uint8_t val) {
  return out << static_cast<uint32_t>(val);
}

inline std::ostream& operator<<(std::ostream& out, const Color& c) {
  return out << c.red_ << ' ' << c.green_ << ' ' << c.blue_;
}

inline bool operator==(const Color& lhs, const Color& rhs) {
  return lhs.red_ == rhs.red_ && lhs.blue_ == rhs.blue_ &&
         lhs.green_ == rhs.green_;
}

namespace colors {
const Color kNoColor = Color{};
const Color kBlack = Color{0, 0, 0};
const Color kWhite = Color{255, 255, 255};
const Color kRed = Color{255, 0, 0};
const Color kGreen = Color{0, 255, 0};
const Color kBlue = Color{0, 0, 255};
const Color kGrey = Color{128, 128, 128};
const Color kOrange = Color{255, 128, 0};
const Color kYellow = Color{255, 255, 0};
const Color kLightGreen = Color{128, 255, 0};
const Color kPaleGreen = Color{0, 255, 128};
const Color kCayan = Color{0, 255, 255};
const Color kLightBlue = Color{0, 128, 255};
const Color kViolet = Color{255, 0, 255};
const Color kPurple = Color{255, 0, 128};
}  // namespace colors

#endif
