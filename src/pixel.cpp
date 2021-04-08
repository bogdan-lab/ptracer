
#include "Pixel.h"


size_t Pixel::SAMPLE_NUM = 1;


void Pixel::TracePixel(const Scene& universe, size_t seed) {
    Renderer render(seed);
    std::vector<Color> accum_colors_;
    accum_colors_.reserve(SAMPLE_NUM);
    for(uint8_t i=0; i<SAMPLE_NUM; i++) {
        accum_colors_.push_back(render.RenderRay(ray_, universe));
    }
    color_ = Renderer::GetAverageColor(accum_colors_);
}

