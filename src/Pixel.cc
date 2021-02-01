#include <xenonled/Pixel.hh>
#include <stdexcept>

int& xenon::Pixel::clamp(int& v){
	if(v < 0){
		v = 0;
	}else if(v > 255){
		v = 255;
	}
	return v;
}

xenon::Pixel& xenon::Pixel::clamp(Pixel& p){
	clamp(p._r);
	clamp(p._g);
	clamp(p._b);
	return p;
}

xenon::Pixel::Pixel() = default;

xenon::Pixel::Pixel(int red, int green, int blue) : _r(red), _g(green), _b(blue){
	clamp(*this);
}

xenon::Pixel::Pixel(const xenon::Pixel& p) : _r(p._r), _g(p._g), _b(p._b){
	clamp(*this);
}

xenon::Pixel::Pixel(Pixel&& p) noexcept : _r(p._r), _g(p._g), _b(p._b){
	clamp(*this);
}

xenon::Pixel& xenon::Pixel::operator=(const Pixel& p){
	_r = p._r;
	_g = p._g;
	_b = p._b;
	return *this;
}

xenon::Pixel::~Pixel(){}

int& xenon::Pixel::operator[](int p){
	switch(p){
		case 0:
			return _r;
		case 1:
			return _g;
		case 2:
			return _b;
	}
	throw std::out_of_range("accessing pixel value not between 0-2");
}

xenon::Pixel xenon::Pixel::operator+(const xenon::Pixel& p){
	Pixel t;
	t._r = _r + p._r;
	t._g = _g + p._g;
	t._b = _b + p._b;
	clamp(t);
	return t;
}

xenon::Pixel xenon::Pixel::operator-(const Pixel& p){
	Pixel t;
	t._r = _r - p._r;
	t._g = _g - p._g;
	t._b = _b - p._b;
	clamp(t);
	return t;
}

xenon::Pixel& xenon::Pixel::operator+=(const Pixel& p){
	_r += p._r;
	_g += p._g;
	_b += p._b;
	return clamp(*this);
}

xenon::Pixel& xenon::Pixel::operator-=(const Pixel& p){
	_r -= p._r;
	_g -= p._g;
	_b -= p._b;
	return clamp(*this);
}

int xenon::Pixel::r(){ return _r; }

int xenon::Pixel::g(){ return _g; }

int xenon::Pixel::b(){ return _b; }