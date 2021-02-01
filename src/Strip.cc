#include <xenonled/Strip.hh>
#include <stdexcept>

xenon::Strip::Strip() : pin(0), size(1), hiddenArray(new Pixel[1]), shownArray(new Pixel[1]), name("NONAME"){}

xenon::Strip::Strip(std::string n, int s, int p, std::string t) : type(t), pin(p), size(s), name(n), hiddenArray(new Pixel[s]), shownArray(new Pixel[s]){}

void xenon::Strip::set(int index, const Pixel& p){
	if(index >= size){
		throw std::runtime_error("strip array index >= strip size");
	}
    hiddenArray[index] = p;
}

void xenon::Strip::set(int index, int r, int g, int b){
	if(index >= size){
		throw std::runtime_error("strip array index >= strip size");
	}
    hiddenArray[index] = Pixel(r, g ,b);
}

xenon::Pixel* xenon::Strip::get(int index){
	if(index >= size){
		return nullptr;
	}
	return &hiddenArray[index];
}

void xenon::Strip::show(){
	for(int a = 0; a < size; ++a){
		shownArray[a] = hiddenArray[a];
	}
}

const xenon::Pixel* xenon::Strip::getShown(int index){
	if(index >= size){
		return nullptr;
	}
	return &shownArray[index];
}