#pragma once

#include "vec3.hpp"

class ray {
public:
	ray() {}
	ray(const point3& origin, const vec3& direction) :og(origin), dir(direction) {};
	const point3& origin() const { return og; }
	const vec3& direction() const { return dir; }
	point3 at(float t) const {
		return og + t * dir;
	}
private:
	point3 og;
	vec3 dir;
};