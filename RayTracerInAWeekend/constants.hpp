#pragma once

#include <iostream>
#include <random>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;
using std::fabs;

static constexpr auto ASPECT_RATIO = 16.0 / 9.0;
static constexpr auto SAMPLES_PER_PIXEL = 100;
static constexpr float INF = std::numeric_limits<float>::infinity();
static constexpr float PI = 3.1415926535897932385f;
static constexpr auto MAX_DEPTH = 50;


inline float degrees_to_radians(float degrees) { return degrees * PI / 180.0f; }
inline float random_float() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}
inline float random_float(float min, float max) { return min + (max - min) * random_float(); }

#include "color.hpp"
#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"