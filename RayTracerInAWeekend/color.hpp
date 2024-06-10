#pragma once
#include "vec3.hpp"
#include "interval.hpp"

using color = vec3;
static constexpr float INT_MAP = 255.999f; // Translate the [0,1] component values to the byte range [0,255].

inline float linear_to_gamma(float linear_component){
	return (linear_component > 0) ? sqrt(linear_component) : 0;
}

void write_color(std::ostream& out, const color& pixel_color) {
	auto r = pixel_color.x;
	auto g = pixel_color.y;
	auto b = pixel_color.z;
	// Apply a linear to gamma transform for gamma 2
	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	static const interval intensity(0.000f, 0.999f);
	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(256 * intensity.clamp(g));
	int bbyte = int(256 * intensity.clamp(b));

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}