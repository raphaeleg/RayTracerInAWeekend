#pragma once
#include "vec3.hpp"
#include "interval.hpp"

using color = vec3;
static constexpr double INT_MAP = 255.999; // Translate the [0,1] component values to the byte range [0,255].

void write_color(std::ostream& out, const color& pixel_color) {
	auto r = pixel_color.x;
	auto g = pixel_color.y;
	auto b = pixel_color.z;

	static const interval intensity(0.000, 0.999);
	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(256 * intensity.clamp(g));
	int bbyte = int(256 * intensity.clamp(b));

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}