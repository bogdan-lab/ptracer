#ifndef PIXEL_H
#define PIXEL_H

#include "Ray.h"
#include "Color.h"
#include "Scene.h"
#include "Renderer.h"

#include <cstdlib>
#include <vector>


class Pixel {
private:
    static size_t SAMPLE_NUM;
    Ray ray_;
    std::vector<Color> accumulated_colors_;
public:
    Pixel() = delete;

    static void SetSampleNum(size_t g_num) {SAMPLE_NUM = g_num;}

    void TracePixel(const Scene& universe, size_t seed);

};




#endif //PIXEL_H
