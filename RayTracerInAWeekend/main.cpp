#include <iostream>
#include <vector>
#include <print>

struct coordinate
{
	int x;
	int y;

	coordinate(int _x, int _y) : x(_x), y(_y) {};
};

static constexpr double INT_MAP = 255.999; // to scale from 0~1 to 0~255
int main() {
	coordinate imgSize { 256, 256 };

	std::cout << "P3\n" << imgSize.x << ' ' << imgSize.y << "\n255\n";

	for (double i = 0; i < imgSize.x; i++) {
		std::clog << "\rScanlines remaining: " << (imgSize.y - i) << ' ' << std::flush;
		for (double j = 0; j < imgSize.y; j++) {
			auto r = j / (imgSize.y - 1);
			auto g = i / (imgSize.x - 1);
			int ir = INT_MAP * r;
			int ig = INT_MAP * g;
			int ib = 0;
			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}
	std::clog << "\rDone.\n";
}