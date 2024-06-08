#include <iostream>
#include "color.hpp"
#include "vec3.hpp"

struct coordinate
{
	int x;
	int y;

	coordinate(int _x, int _y) : x{ _x }, y{ _y } {};
};

int main() {
	coordinate imgSize { 256, 256 };

	std::cout << "P3\n" << imgSize.x << ' ' << imgSize.y << "\n255\n";

	for (float i = 0; i < imgSize.x; i++) {
		std::clog << "\rScanlines remaining: " << (imgSize.y - i) << ' ' << std::flush;
		for (float j = 0; j < imgSize.y; j++) {
			auto pixel_color = color(j / (imgSize.x - 1), i / (imgSize.y - 1), 0);
			write_color(std::cout, pixel_color);
		}
	}
	std::clog << "\rDone.               \n";
}