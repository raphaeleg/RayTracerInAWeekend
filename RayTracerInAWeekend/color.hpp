#pragma once

#include "vec3.hpp"
#include <iostream>

using color = vec3;
static constexpr double INT_MAP = 255.999; // Translate the [0,1] component values to the byte range [0,255].

void write_color(std::ostream& out, const color& pixel_color) {
	auto r = pixel_color.x;
	auto g = pixel_color.y;
	auto b = pixel_color.z;

	int rbyte = int(INT_MAP * r);
	int gbyte = int(INT_MAP * g);
	int bbyte = int(INT_MAP * b);

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}