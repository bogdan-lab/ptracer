#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <random>

#include "GeoVec.h"

GeoVec MirrorReflect(const GeoVec& dir, const GeoVec& norm);

GeoVec DiffuseReflect(std::mt19937& rnd, const GeoVec& dir, const GeoVec& norm);

GeoVec HybridReflect(std::mt19937& rnd, double polishness, const GeoVec& dir,
                     const GeoVec& norm);

#endif  // REFLECTOR_H
