#include "constants.hpp"

#include "camera.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"

int main() {
	hittable_list world;
	
    auto material_ground = make_shared<lambertian>(color(0.8f, 0.8f, 0.0f));
    auto material_center = make_shared<lambertian>(color(0.1f, 0.2f, 0.5f));
    auto material_left = make_shared<dielectric>(1.50f);
    auto material_bubble = make_shared<dielectric>(1.00f / 1.50f);
    auto material_right = make_shared<metal>(color(0.8f, 0.6f, 0.2f), 1.0f);

    world.add(make_shared<sphere>(point3(0.0f, -100.5f, -1.0f), 100.0f, material_ground));
    world.add(make_shared<sphere>(point3(0.0f, 0.0f, -1.2f), 0.5f, material_center));
    world.add(make_shared<sphere>(point3(-1.0f, 0.0f, -1.0f), 0.5f, material_left));
    world.add(make_shared<sphere>(point3(-1.0f, 0.0f, -1.0f), 0.4f, material_bubble));
    world.add(make_shared<sphere>(point3(1.0f, 0.0f, -1.0f), 0.5f, material_right));

    point3 lf(-2.0f, 2.0f, 1.0f);
    point3 la(0.0f, 0.0f, -1.0f);
    vec3 vu(0.0f, 1.0f, 0.0f);
    camera cam(lf,la,vu, 20.0f,10.0f,3.4f);
	cam.render(world);
}