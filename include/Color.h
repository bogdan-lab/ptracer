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

  constexpr Color() = default;

  constexpr Color(int64_t R, int64_t G, int64_t B) : is_color_(1) {
    red_ = ColorClamp(R);
    green_ = ColorClamp(G);
    blue_ = ColorClamp(B);
  }

  constexpr uint8_t ColorClamp(int64_t val) noexcept {
    auto high = std::numeric_limits<uint8_t>::max();
    auto low = std::numeric_limits<uint8_t>::min();
    return static_cast<uint8_t>(val >= high ? high : (val <= low ? low : val));
  }

  constexpr void TruncByColor(const Color& tr_col) {
    if (!IsColor()) return;
    if (red_ > tr_col.red_) red_ = tr_col.red_;
    if (green_ > tr_col.green_) green_ = tr_col.green_;
    if (blue_ > tr_col.blue_) blue_ = tr_col.blue_;
  }

  constexpr bool IsColor() const { return is_color_ == 1; }
};

inline constexpr bool operator==(const Color& lhs, const Color& rhs) {
  return lhs.red_ == rhs.red_ && lhs.blue_ == rhs.blue_ &&
         lhs.green_ == rhs.green_;
}

namespace colors {
constexpr Color kNoColor{};
constexpr Color kBlack{0, 0, 0};
constexpr Color kWhite{255, 255, 255};
constexpr Color kRed{255, 0, 0};
constexpr Color kGreen{0, 255, 0};
constexpr Color kBlue{0, 0, 255};
constexpr Color kGrey{128, 128, 128};
constexpr Color kOrange{255, 128, 0};
constexpr Color kYellow{255, 255, 0};
constexpr Color kLightGreen{128, 255, 0};
constexpr Color kPaleGreen{0, 255, 128};
constexpr Color kCayan{0, 255, 255};
constexpr Color kLightBlue{0, 128, 255};
constexpr Color kViolet{255, 0, 255};
constexpr Color kPurple{255, 0, 128};
}  // namespace colors

#endif
