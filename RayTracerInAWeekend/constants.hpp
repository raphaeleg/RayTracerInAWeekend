#pragma once

#include <iostream>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

static constexpr auto ASPECT_RATIO = 16.0 / 9.0;
static constexpr float INF = std::numeric_limits<float>::infinity();
static constexpr float PI = 3.1415926535897932385f;

inline float degrees_to_radians(float degrees) {
    return degrees * PI / 180.0;
}

#include "color.hpp"
#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"