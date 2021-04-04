
#include "Pixel.h"


size_t Pixel::SAMPLE_NUM = 1;


void Pixel::TracePixel(const Scene& universe, size_t seed) {
    Renderer render(seed);
    accumulated_colors_.reserve(SAMPLE_NUM);
    for(uint8_t i=0; i<SAMPLE_NUM; i++) {
        accumulated_colors_.push_back(render.RenderRay(ray_, universe));
    }
}

