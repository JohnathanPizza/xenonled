#include <xenonled/Var.hh>
#include <xenonled/Pixel.hh>
#include <string>
#include <stdexcept>

std::string xenon::Var::getName() const{ return name; }

xenon::Var::Var(){
	name = "NONAME";
	i = 0;
	type = VTYPE::Vint;
}

xenon::Var::~Var(){}

xenon::Var::Var(std::string n) : name(n), i(0), type(VTYPE::Vint){}

xenon::Var::Var(const Var& u){
	type = u.type;
	switch(type){
		case VTYPE::Vint:
			i = u.i; break;
		case VTYPE::Vdouble:
			d = u.d; break;
	}
	name = u.name;
}

xenon::Var& xenon::Var::operator=(const Var& v){
	name = v.name;
	i = v.i;
	type = v.type;
	return *this;
}

void xenon::Var::set(int v){
	type = VTYPE::Vint;
	i = v;
}

void xenon::Var::set(double v){
	type = VTYPE::Vdouble;
	d = v;
}

VTYPE xenon::Var::getType() const { return type; }

template<typename T>
T& xenon::Var::getVal(){}

template<>
int& xenon::Var::getVal<int>(){
	if(type == VTYPE::Vint){
		return i;
	}
	throw std::invalid_argument("varaible type not int");
}

template<>
double& xenon::Var::getVal<double>(){
	if(type == VTYPE::Vdouble){
		return d;
	}
	throw std::invalid_argument("varaible type not double");
}