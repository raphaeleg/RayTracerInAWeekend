#include <iostream>
#include "color.hpp"
#include "ray.hpp"
#include "vec3.hpp"

static constexpr auto ASPECT_RATIO = 16.0 / 9.0;

struct Dimension
{
	float w;
	float h;

	Dimension() : w(0), h(0) {};
	Dimension(float _w, float _h) : w( _w ), h( _h ) {};
};

struct Camera {
	float focal_length = 1.0;
	Dimension viewport;
	point3 center{0,0,0};
	vec3 viewport_x;
	vec3 viewport_y;
	vec3 pixel_delta_x;
	vec3 pixel_delta_y;
	vec3 upperLeftViewport;
	vec3 pixel00_loc;

	Camera(float viewport_height, Dimension image) {
		viewport = Dimension{ viewport_height * float(ASPECT_RATIO), viewport_height };
		viewport_x = vec3(viewport.w, 0, 0);
		viewport_y = vec3(0, -viewport.h, 0);
		pixel_delta_x = viewport_x / image.w;
		pixel_delta_y = viewport_y / image.h;
		upperLeftViewport = center - vec3(0,0, focal_length)-viewport_x/2-viewport_y/2;
		pixel00_loc = upperLeftViewport + 0.5 * (pixel_delta_x + pixel_delta_y);
	}
};

static bool hit_sphere(const point3& center, double radius, const ray& r) {
	vec3 oc = center - r.origin();
	auto a = dot(r.direction(), r.direction());
	auto b = -2.0 * dot(r.direction(), oc);
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = b * b - 4 * a * c;
	return (discriminant >= 0);
}

color ray_color(const ray& r) {
	if (hit_sphere(point3(0, 0, -1), 0.5, r)) {
		return color(1, 0, 0);
	}
	vec3 unit_direction = unit_vector(r.direction());
	auto a = 0.5 * (unit_direction.y + 1.0);
	return (1.0 - a) * vec3(1.0f) + a * vec3(0.5f, 0.7f, 1.0f);
}

int main() {
	float img_width = 400;
	Dimension image { img_width, float(std::max(1, int(img_width / ASPECT_RATIO))) };
	Camera cam{2.0f,image};
	std::clog << "image: " << img_width << "x"<< int(img_width / ASPECT_RATIO) << "\nviewport: "<<cam.viewport.w<<"x"<<cam.viewport.h<<"\n";

	std::cout << "P3\n" << image.w << ' ' << image.h << "\n255\n";

	for (int y = 0; y < image.h; y++) {
		std::clog << "\rScanlines remaining: " << (image.h - y) << ' ' << std::flush;
		for (int x = 0; x < image.w; x++) {
			auto pixel_center = cam.pixel00_loc + (x * cam.pixel_delta_x) + (y * cam.pixel_delta_y);
			auto ray_direction = pixel_center - cam.center;
			ray r(cam.center, ray_direction); // could be made into a unit vector

			color pixel_color = ray_color(r);
			write_color(std::cout, pixel_color);
		}
	}
	std::clog << "\rDone.                      \n";
}