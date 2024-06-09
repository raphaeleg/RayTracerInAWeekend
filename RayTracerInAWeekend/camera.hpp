#pragma once
#include "constants.hpp"
#include "hittable.hpp"

struct Dimension
{
	float w;
	float h;

	Dimension() : w(0), h(0) {};
	Dimension(float _w, float _h) : w(_w), h(_h) {};
};

class camera {
public:
	float img_width = 400;
	Dimension image{ img_width, float(std::max(1, int(img_width / ASPECT_RATIO))) };

	camera(float viewport_height) {
		viewport = Dimension{ viewport_height * float(ASPECT_RATIO), viewport_height };
		viewport_x = vec3(viewport.w, 0, 0);
		viewport_y = vec3(0, -viewport.h, 0);
		pixel_delta_x = viewport_x / image.w;
		pixel_delta_y = viewport_y / image.h;
		upperLeftViewport = center - vec3(0, 0, focal_length) - viewport_x / 2 - viewport_y / 2;
		pixel00_loc = upperLeftViewport + 0.5 * (pixel_delta_x + pixel_delta_y);
	}

	void render(const hittable& world) {
		std::clog << "image: " << img_width << "x" << int(img_width / ASPECT_RATIO) << "\nviewport: " << viewport.w << "x" << viewport.h << "\n";
		std::cout << "P3\n" << image.w << ' ' << image.h << "\n255\n";

		for (int y = 0; y < image.h; y++) {
			std::clog << "\rScanlines remaining: " << (image.h - y) << ' ' << std::flush;
			for (int x = 0; x < image.w; x++) {
				auto pixel_center = pixel00_loc + (x * pixel_delta_x) + (y * pixel_delta_y);
				auto ray_direction = pixel_center - center;
				ray r(center, ray_direction); // could be made into a unit vector

				color pixel_color = ray_color(r, world);
				write_color(std::cout, pixel_color);
			}
		}
		std::clog << "\rDone.                      \n";
	}
private:
	float focal_length = 1.0;
	Dimension viewport;
	point3 center{ 0,0,0 };
	vec3 viewport_x;
	vec3 viewport_y;
	vec3 pixel_delta_x;
	vec3 pixel_delta_y;
	vec3 upperLeftViewport;
	vec3 pixel00_loc;
	
	color ray_color(const ray& r, const hittable& world) const {
		hit_record rec;
		if (world.hit(r, interval(0, INF), rec)) {
			return 0.5 * (rec.normal + color(1, 1, 1));
		}

		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}
};