#include "PngWriter.h"

#include <vector>

#include "Color.h"

PngWriter::PngWriter(const char *filename, uint32_t width, uint32_t height)
    : file_(fopen(filename, "wb"), fclose),
      img_width_(width),
      img_height_(height) {
  if (!file_) return;
  png_struct_ =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

  if (!png_struct_) return;

  info_ = png_create_info_struct(png_struct_);
  if (!info_) return;

  png_init_io(png_struct_, file_.get());

  png_set_IHDR(png_struct_, info_, img_width_, img_height_, 8,
               PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_struct_, info_);
}

PngWriter::~PngWriter() {
  png_write_end(png_struct_, info_);
  png_destroy_write_struct(&png_struct_, &info_);
}

void PngWriter::SaveVectorToImage(const std::vector<Color> &vec) {
  uint8_t row[3 * img_width_];
  size_t px_num = img_height_ * img_width_;
  size_t r_idx = 0;
  for (size_t i = 0; i < px_num; i++) {
    row[r_idx++] = vec[i].red_;
    row[r_idx++] = vec[i].green_;
    row[r_idx++] = vec[i].blue_;
    if (!((i + 1) % img_width_)) {
      r_idx = 0;
      png_write_row(png_struct_, row);
    }
  }
}
