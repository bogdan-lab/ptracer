#ifndef PNG_WRITER_H
#define PNG_WRITER_H

#include <png.h>

#include <cstdint>
#include <cstdio>
#include <memory>
#include <vector>

#include "Color.h"
/** Simple wrapper for saving png images*/
class PngWriter {
  std::unique_ptr<FILE, decltype(fclose) *> file_;
  png_structp png_struct_{};
  png_infop info_{};

  uint32_t img_width_ = 0;
  uint32_t img_height_ = 0;

 public:
  PngWriter(const char *filename, uint32_t width, uint32_t height);
  ~PngWriter();

  PngWriter(const PngWriter &) = delete;
  PngWriter &operator=(const PngWriter &) = delete;
  PngWriter(PngWriter &&) = delete;
  PngWriter &operator=(PngWriter &&) = delete;

  bool IsOk() const { return png_struct_ && info_; }

  void SaveVectorToImage(const std::vector<Color> &vec);
};

#endif  // PNG_WRITER_H
