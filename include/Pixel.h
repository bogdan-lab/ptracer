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
    Color color_;
public:
    Pixel() = delete;

    explicit Pixel(const Ray& r) : ray_(r), color_(0,0,0) {}

    static void SetSampleNum(size_t g_num) {SAMPLE_NUM = g_num;}

    void TracePixel(const Scene& universe, size_t seed);

    const Color& GetPixelColor() const {return color_;}

};




#endif //PIXEL_H
