/**
 * @file Color.h
 * Contain color utilities
 */
#ifndef COLOR_H
#define COLOR_H

#include <cstdlib>
#include <iostream>
#include <limits>
#include <vector>

// TODO make is_color_field - enum

/**
 * Struct represents single color.
 * Color can have empty state which is marked by is_color_ field.
 */
struct Color {
  uint8_t red_ = 0;
  uint8_t green_ = 0;
  uint8_t blue_ = 0;
  /** Can have only values 1 - valid color is stored, 0 - color is empty*/
  uint8_t is_color_ = 0;

  /**
   * By default empty color is created
   */
  constexpr Color() = default;

  /** Method calculates average color based on given collection of colors.*/
  static Color GetAverageColor(const std::vector<Color>& colors) {
    int64_t r = 0;
    int64_t g = 0;
    int64_t b = 0;
    int64_t col_num = 0;
    for (const auto& el : colors) {
      if (el.IsColor()) {
        r += el.red_;
        g += el.green_;
        b += el.blue_;
        col_num++;
      }
    }
    if (!col_num) return Color{0, 0, 0};
    return {r / col_num, g / col_num, b / col_num};
  }
  /**
   * Light we see is reflection of the light from source.
   * if in light in source red == 0 there could be no red in reflection!
   * The same stays for the reflected light
   * Last note in colors is expected to be light source color -> trunc all
   * colors starting from the end to the beginning
   * Procedure should be preformed inside one ray trace
   */
  static void TruncColorsInTrace(std::vector<Color>& colors) {
    if (colors.empty()) return;
    auto it_source = colors.rbegin();
    for (auto it = std::next(it_source); it != colors.rend();
         ++it, ++it_source) {
      it->TruncByColor(*it_source);
    }
  }
  // TODO do I really need this clamping ???
  /**
   * Creates color based on RGB arguments.
   * passed arguments are clamped from 0 to 255 during creation
   */
  constexpr Color(int64_t R, int64_t G, int64_t B) : is_color_(1) {
    red_ = ColorClamp(R);
    green_ = ColorClamp(G);
    blue_ = ColorClamp(B);
  }

  // TODO why is not it static ?
  /**
   * Clamps given val between 0 and 255 and converts it to uint8_t
   */
  constexpr uint8_t ColorClamp(int64_t val) noexcept {
    auto high = std::numeric_limits<uint8_t>::max();
    auto low = std::numeric_limits<uint8_t>::min();
    return static_cast<uint8_t>(val >= high ? high : (val <= low ? low : val));
  }
  /**
   * This method clamps the current color values by those of the given one.
   * This is, if argument color has in red value 100, than current color red
   * value is clamped between 0 and 100.
   */
  constexpr void TruncByColor(const Color& tr_col) {
    if (!IsColor()) return;
    if (red_ > tr_col.red_) red_ = tr_col.red_;
    if (green_ > tr_col.green_) green_ = tr_col.green_;
    if (blue_ > tr_col.blue_) blue_ = tr_col.blue_;
  }
  /** Checks if current color is empty*/
  constexpr bool IsColor() const { return is_color_ == 1; }
};

inline constexpr bool operator==(const Color& lhs, const Color& rhs) {
  return lhs.red_ == rhs.red_ && lhs.blue_ == rhs.blue_ &&
         lhs.green_ == rhs.green_;
}

inline constexpr bool operator!=(const Color& lhs, const Color& rhs) {
  return !(lhs == rhs);
}
/** Namespace with color constants*/
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
