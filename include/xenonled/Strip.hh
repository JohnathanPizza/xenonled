#ifndef STRIP_H
#define STRIP_H

#include <xenonled/Pixel.hh>
#include <string>
#include <memory>

namespace xenon{
	class Strip{
		const int size;
		const int pin;
		std::string name;
		std::string type;
		std::unique_ptr<xenon::Pixel[]> hiddenArray;
		std::unique_ptr<xenon::Pixel[]> shownArray;
	public:
		Strip();
		Strip(std::string n, int s, int p, std::string t = "");
		inline int getSize(){ return size; }
		inline int getPin(){ return pin; }
		inline std::string getName(){ return name; }
		inline std::string getType(){ return type; }
		void set(int index, const xenon::Pixel& p);
		void set(int index, int r, int g, int b);
		xenon::Pixel* get(int index);
		void show();
		const xenon::Pixel* getShown(int index = 0);
	};
}

#endif