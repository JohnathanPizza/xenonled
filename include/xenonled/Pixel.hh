#ifndef PIXEL_H
#define PIXEL_H

#include <array>

namespace xenon{
	struct Pixel{
	private:
		int& clamp(int& v);
		Pixel& clamp(Pixel& p);
		int _r = 0;
		int _g = 0;
		int _b = 0;
	public:
		Pixel();
		Pixel(int red, int green, int blue);
		Pixel(const Pixel& p);
		Pixel(Pixel&& p) noexcept;
		Pixel& operator=(const Pixel& p);
		~Pixel();
		int& operator[](int p);
		Pixel operator+(const Pixel& p);
		Pixel operator-(const Pixel& p);
		Pixel& operator+=(const Pixel& p);
		Pixel& operator-=(const Pixel& p);
		int r();
		int g();
		int b();
	};
}

#endif