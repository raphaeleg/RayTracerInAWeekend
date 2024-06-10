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
	float img_width = 1200;
	Dimension image{ img_width, float(fmax(1, int(img_width / ASPECT_RATIO))) };

	camera(point3 lf, point3 la, vec3 vu, float vf, float defocus_angle, float focus_d) {
		lookfrom = lf;
		lookat = la;
		vup = vu;
		v_fov = vf;
		dof_angle = defocus_angle;
		focus_dist = focus_d;

		center = lookfrom;

		auto theta = degrees_to_radians(v_fov);
		auto h = tan(theta / 2);
		auto viewport_height = 2 * h * focus_dist;
		viewport = Dimension{ viewport_height * float(ASPECT_RATIO), viewport_height };

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		viewport_x = viewport.w * u;
		viewport_y = viewport.h * -v;
		pixel_delta_x = viewport_x / image.w;
		pixel_delta_y = viewport_y / image.h;

		auto upperLeftViewport = center - (focus_dist * w) - viewport_x / 2 - viewport_y / 2;
		pixel00_loc = upperLeftViewport + 0.5 * (pixel_delta_x + pixel_delta_y);

		auto defocus_radius = focus_dist * tan(degrees_to_radians(dof_angle / 2));
		dof_disk_x = u * defocus_radius;
		dof_disk_y = v * defocus_radius;
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
	float focal_length = 1.0f;
	point3 lookfrom{ 0.0f };   // Point camera is looking from
	point3 lookat = { 0.0f, 0.0f, -1.0f };  // Point camera is looking at
	vec3 vup{ 0.0f, 1.0f, 0.0f };     // Camera-relative "up" direction
	float dof_angle = 0.0f;
	float focus_dist = 10.0f;
	vec3 dof_disk_x{ 0.0 };
	vec3 dof_disk_y{ 0.0 };
	
	Dimension viewport;
	point3 center{ 0.0f };
	vec3 viewport_x{ 0.0f };
	vec3 viewport_y{ 0.0f };
	vec3 pixel_delta_x{ 0.0f };
	vec3 pixel_delta_y{ 0.0f };
	vec3 pixel00_loc{ 0.0f };
	float pixel_samples_scale = float(1.0f / SAMPLES_PER_PIXEL);

	float v_fov = 20.0f;
	vec3 u{ 0.0f };
	vec3 v{ 0.0f };
	vec3 w{ 0.0f };
	
	ray get_ray(int i, int j) const {
		auto offset = sample_square();
		auto pixel_sample = pixel00_loc
			+ ((i + offset.x) * pixel_delta_x)
			+ ((j + offset.y) * pixel_delta_y);

		auto ray_origin = (dof_angle <= 0) ? center : defocus_disk_sample();
		auto ray_direction = pixel_sample - ray_origin;

		return ray(ray_origin, ray_direction);
	}

	vec3 sample_square() const {
		return vec3(random_float() - 0.5f, random_float() - 0.5f, 0.0f);
	}
	point3 defocus_disk_sample() const {
		auto p = random_in_unit_disk();
		return center + (p[0] * dof_disk_x) + (p[1] * dof_disk_y);
	}

	color background_render(const ray& r) const {
		vec3 unit_direction = unit_vector(r.direction());
		float a = 0.5f * (unit_direction.y + 1.0f);
		return (1.0f - a) * color(1.0f, 1.0f, 1.0f) + a * color(0.5f, 0.7f, 1.0f); // blend
	}

	color ray_color(const ray& r, int depth, const hittable& world) const {
		if (depth <= 0) { return color(0, 0, 0); }
		hit_record rec;

		bool obj_hit = world.hit(r, interval(0.001f, INF), rec);

		if (!obj_hit) { return background_render(r); }

		ray scattered;
		color attenuation;
		if (rec.mat->scatter(r, rec, attenuation, scattered)) {
			return attenuation * ray_color(scattered, depth - 1, world);
		}
		return color(0, 0, 0);
	}
	
};
