
#include "Pixel.h"




void Pixel::TracePixel(const Scene& universe) {
    Renderer render;
    std::vector<Color> accum_colors_;
    accum_colors_.reserve(in_rays_.size());
    for(const auto& ray : in_rays_) {
        accum_colors_.push_back(render.RenderRay(ray, universe));
    }
    color_ = Renderer::GetAverageColor(accum_colors_);
}

