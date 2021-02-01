#include <xenonled/inogen.hh>
#include <xenonled/essential.hh>
#include <fstream>
#include <xenonled/Pixel.hh>
#include <iostream>
#include <algorithm>

void xenon::inogen(std::string name){
	std::ofstream file(name, std::ios_base::out);
	std::vector<std::string> strips;
	std::vector<std::string> pixels;
	std::string activeStrip;
	int tabs = 0;
	//puts tabs into file plus string
	auto fout = [&](std::string s){
		for(int t = tabs; t > 0; --t) file << "\t";
		if(tabs < 0) tabs = 0;
		file << s;
	};
	fout("#include <Adafruit_NeoPixel.h>\n");
	size_t pos = 0;
	for(; pos < xenon::rawList.size(); ++pos){
		const RawCommand_t& c = xenon::rawList.at(pos);
		if(c.first == "i"){
			fout("int " + c.second.at(0) + " = " + c.second.at(1) + ";\n");
		}else if(c.first == "d"){
			fout("float " + c.second.at(0) + " = " + c.second.at(1) + ";\n");
		}else if(c.first == "newstrip"){
			fout("Adafruit_NeoPixel " + c.second.at(0) + "(" + c.second.at(1) + ", " + c.second.at(2) + ");\n");
			strips.emplace_back(c.second.at(0));
		}else if(c.first == "tag"){
			if(c.second.at(0) == "setloop"){
				fout("void setup(){\n");
				++tabs;
				for(const std::string& strip : strips){
					fout(strip + ".begin();\n");
					fout(strip + ".show();\n");
				}
				--tabs;
				fout("}\n\nvoid loop(){\n");
				++tabs;
			}else if(c.second.at(0) == "com"){
				fout("//" + c.second.at(1));
			}else if(c.second.at(0) == "add"){
				fout(c.second.at(1));
			}else if(c.second.at(0) == "endloop"){
				--tabs;
				fout("}\n");
			}
		}else if(c.first == "set"){
			fout(c.second.at(0) + " = " + c.second.at(1) + ";\n");
		}else if(c.first == "if"){
			fout("if(" + c.second.at(0) + "){\n");
			++tabs;
		}else if(c.first == "loop"){
			fout("while(" + c.second.at(0) + "){\n");
			++tabs;
		}else if(c.first == "end"){
			--tabs;
			fout("}\n");
		}else if(c.first == "elseif"){
			fout("else if(" + c.second.at(0) + "){\n");
			++tabs;
		}else if(c.first == "setstrip"){
			activeStrip = c.second.at(0);
		}else if(c.first == "show"){
			fout(activeStrip + ".show();\n");
		}else if(c.first == "setpix"){
			if(c.second.size() == 3){
				fout(activeStrip + ".setPixelColor(" + c.second.at(0) + ", " + c.second.at(1) + ", " + c.second.at(2) + ", " + c.second.at(3) + ");\n");
			}else if(c.second.size() == 1){
				fout(activeStrip + ".setPixelColor(" + c.second.at(0) + ", " + c.second.at(1) + ", " + c.second.at(1) + ", " + c.second.at(1) + ");\n");
			}
		}else if(c.first == "setall"){
			if(c.second.size() == 1){
				fout(activeStrip + ".fill(" + activeStrip + ".color(" + c.second.at(0) + ", " + c.second.at(0) + ", " + c.second.at(0) + "));\n");
			}else if(c.second.size() == 3){
				fout(activeStrip + ".fill(" + activeStrip + ".color(" + c.second.at(0) + ", " + c.second.at(1) + ", " + c.second.at(3) + "));\n");
			}
		}else if(c.first == "break"){
			fout("break;\n");
		}else if(c.first == "continue"){
			fout("continue;\n");
		}else if(c.first == "else"){
			fout("else{\n");
			++tabs;
		}else if(c.first == "swapd"){
			fout("uint32_t _COL1_ = " + activeStrip + ".getPixelColor(" + c.second.at(0) + ");\n");
			fout(activeStrip + ".setPixelColor(" + c.second.at(0) + ", " + activeStrip + ".getPixelColor(" + c.second.at(1) + "));\n");
			fout(activeStrip + ".setPixelColor(" + c.second.at(1) + ", _COL1_);\n");
		}
	}
	file.close();
}