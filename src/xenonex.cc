#include <xenonled/xenonex.hh>

xenon::XenonEx::XenonEx() : val("error"){}

xenon::XenonEx::XenonEx(const std::string& s) : val(s){}

const char* xenon::XenonEx::what() const noexcept{
	return val.c_str();
}

xenon::XenonEx& xenon::XenonEx::operator=(const std::exception& e){
	val = e.what();
	return *this;
}