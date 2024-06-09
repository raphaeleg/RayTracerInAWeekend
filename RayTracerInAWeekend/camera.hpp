#pragma once
#include "constants.hpp"
#include "hittable.hpp"
#include "material.hpp"

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
		pixel_samples_scale = 1.0 / SAMPLES_PER_PIXEL;
	}

	void render(const hittable& world) {
		std::clog << "image: " << img_width << "x" << int(img_width / ASPECT_RATIO) << "\nviewport: " << viewport.w << "x" << viewport.h << "\n";
		std::cout << "P3\n" << image.w << ' ' << image.h << "\n255\n";

		for (int y = 0; y < image.h; y++) {
			std::clog << "\rScanlines remaining: " << (image.h - y) << ' ' << std::flush;
			for (int x = 0; x < image.w; x++) {
				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < SAMPLES_PER_PIXEL; sample++) {
					ray r = get_ray(x,y);
					pixel_color += ray_color(r,MAX_DEPTH, world);
				}
				write_color(std::cout, pixel_samples_scale * pixel_color);
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
	float pixel_samples_scale;  // Color scale factor for a sum of pixel samples
	
	ray get_ray(int i, int j) const {
		// Construct a camera ray originating from the origin and directed at randomly sampled
		// point around the pixel location i, j.

		auto offset = sample_square();
		auto pixel_sample = pixel00_loc
			+ ((i + offset.x) * pixel_delta_x)
			+ ((j + offset.y) * pixel_delta_y);

		auto ray_direction = pixel_sample - center;

		return ray(center, ray_direction);
	}

	vec3 sample_square() const {
		// Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
		return vec3(random_float() - 0.5, random_float() - 0.5, 0);
	}

	color background_render(const ray& r) const {
		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0); // blend
	}

	color ray_color(const ray& r, int depth, const hittable& world) const {
		if (depth <= 0) { return color(0, 0, 0); }
		hit_record rec;

		bool obj_hit = world.hit(r, interval(0.001, INF), rec);

		if (!obj_hit) { return background_render(r); }

		ray scattered;
		color attenuation;
		if (rec.mat->scatter(r, rec, attenuation, scattered)) {
			return attenuation * ray_color(scattered, depth - 1, world);
		}
		return color(0, 0, 0);
	}
	
};
