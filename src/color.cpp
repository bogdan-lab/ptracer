#include "Color.h"

Color Color::GetAverageColor(const std::vector<Color>& colors) {
  int64_t r = 0;
  int64_t g = 0;
  int64_t b = 0;
  int64_t col_num = 0;
  for (const auto& el : colors) {
    if (el.IsColor()) {
      r += el.red;
      g += el.green;
      b += el.blue;
      col_num++;
    }
  }
  if (!col_num) return Color{0, 0, 0};
  return {r / col_num, g / col_num, b / col_num};
}

void Color::TruncColorsInTrace(std::vector<Color>& colors) {
  if (colors.empty()) return;
  auto it_source = colors.rbegin();
  for (auto it = std::next(it_source); it != colors.rend(); ++it, ++it_source) {
    it->TruncByColor(*it_source);
  }
}
