#include <xenonled/command.hh>
#include <iostream>
#include <algorithm>
#include <stack>
#include <xenonled/Pixel.hh>
#include <xenonled/essential.hh>
#include <xenonled/Strip.hh>
#include <xenonled/Var.hh>
#include <xenonled/argManip.hh>
#include <xenonled/VarManager.hh>
#include <xenonled/xenonex.hh>

using ArgsIn_t = std::vector<std::string>;

namespace xenonCommands{
	xenon::VarManager varManager;
}

//placeholder
void xenonCommands::none(ArgsIn_t){}

//changes a variable's value
void xenonCommands::set(ArgsIn_t args){
	xenon::Var* v = varManager.find(args.at(0));
	if(v){
		try{
			if(v->getType() == VTYPE::Vint){
				v->set(std::stoi(args.at(1)));
			}else if(v->getType() == VTYPE::Vdouble){
				v->set(std::stod(args.at(1)));
			}
		}catch(std::exception& e){
			std::cout << e.what() << std::endl;
		}
	}
}
//sets a pixel on active strip to an rgb value
void xenonCommands::setpix(ArgsIn_t args){
	int i = std::stoi(args.at(0));
	if(i < 0 || i >= xenon::activeStrip->getSize()){
		throw std::out_of_range("data array index not in range");
	}
	xenon::activeStrip->set(i, xenon::Pixel(std::stoi(args.at(1)), std::stoi(args.at(2)), std::stoi(args.at(3))));
}

//create a new int variable
void xenonCommands::i(ArgsIn_t args){
	if(varManager.find(args.at(0))){
		throw std::invalid_argument("cannot create new int with existing name " + args.at(0) + " already exists");
	}
	xenon::Var vu(args.at(0));
	vu.set(std::stoi(args.at(1)));
	varManager.add(vu);
}

//create a new double variable
void xenonCommands::d(ArgsIn_t args){
	if(varManager.find(args.at(0))){
		throw std::invalid_argument("cannot create new double with existing name " + args.at(0) + " already exists");
	}
	xenon::Var vu(args.at(0));
	vu.set(std::stod(args.at(1)));
	varManager.add(vu);
}

//executes code up to next endif on same layer if var != 0
void xenonCommands::_if(ArgsIn_t args){
	varManager.newLayer();
	if(std::stod(args.at(0)) == 0){
		int layer = 1;
		std::string c;
		while(layer != 0){
			++xenon::commandPos;
			c = xenon::rawList.at(xenon::commandPos).first;
			if(c == "if" || c == "loop"){
				++layer;
			}else if(c == "end"){
				--layer;
			}else if((c == "else" || c == "elseif") && layer == 1){
				--layer;
			}
		}
		if(c == "end" || c == "elseif"){
			--xenon::commandPos;
		}
	}
}

//else switch for if statement
void xenonCommands::_else(ArgsIn_t args){
	int layer = 1;
	while(layer != 0){
		++xenon::commandPos;
		std::string c = xenon::rawList.at(xenon::commandPos).first;
		if(c == "if" || c == "loop"){
			++layer;
		}else if(c == "end"){
			--layer;
		}
	}
	--xenon::commandPos;
}

//elseif switch for if statement
void xenonCommands::elseif(ArgsIn_t args){
	if(std::stod(args.at(0)) == 0){
		int layer = 1;
		std::string c;
		while(layer != 0){
			++xenon::commandPos;
			c = xenon::rawList.at(xenon::commandPos).first;
			if(c == "if" || c == "endif"){
				++layer;
			}else if(c == "end"){
				--layer;
			}else if((c == "else" || c == "elseif") && layer == 1){
				--layer;
			}
		}
		if(c == "end" || c == "elseif"){
			--xenon::commandPos;
		}
	}
}


//loops code until next endloop on same layer until var == 0
void xenonCommands::loop(ArgsIn_t args){
	varManager.newLayer();
	if(std::stod(args.at(0)) == 0){
		varManager.eraseLayer();
		int layer = 1;
		while(layer != 0){
			++xenon::commandPos;
			std::string c = xenon::rawList.at(xenon::commandPos).first;
			if(c == "loop" || c == "if"){
				++layer;
			}else if(c == "end"){
				--layer;
			}
		}
	}
}

//marks end of loops and ifs
void xenonCommands::end(ArgsIn_t args){
	int layer = 1;
	int cpos = xenon::commandPos;
	while(layer != 0){
		--xenon::commandPos;
		std::string c = xenon::rawList.at(xenon::commandPos).first;
		if(c == "end"){
			++layer;
		}else if(c == "loop" || c == "if"){
			--layer;
		}
	}
	if(xenon::rawList.at(xenon::commandPos).first == "loop"){
		--xenon::commandPos;
	}else{
		xenon::commandPos = cpos;
	}
	varManager.eraseLayer();
}

//swaps two pixel values in active strip
void xenonCommands::swapd(ArgsIn_t args){
	int a = std::stoi(args.at(0));
	int b = std::stoi(args.at(1));
	if(a < 0 || a >= xenon::activeStrip->getSize() || b < 0 || b >= xenon::activeStrip->getSize()){
		throw std::out_of_range("data array index not in range");
	}
	xenon::Pixel c(*xenon::activeStrip->get(a));
	xenon::activeStrip->set(a, *xenon::activeStrip->get(b));
	*xenon::activeStrip->get(b) = c;
}

//sets all pixels in active strip to a number/pixel value
void xenonCommands::setall(ArgsIn_t args){
	xenon::Pixel p;
	if(args.size() == 1){
		int v = std::stoi(args.at(0));
		p = xenon::Pixel(v, v, v);
	}else if(args.size() == 3){
		
	}
	for(int a = 0; a < xenon::activeStrip->getSize(); ++a){
		*xenon::activeStrip->get(a) = p;
	}
}

//breaks a loop
void xenonCommands::_break(ArgsIn_t args){
	int cpos = xenon::commandPos;
	std::string c = xenon::rawList.at(xenon::commandPos).first;
	int layer = 1;
	while(c != "loop" && layer > 0){
		--xenon::commandPos;
		c = xenon::rawList.at(xenon::commandPos).first;
		if(c == "if" || c == "loop"){
			--layer;
		}else if(c == "end"){
			++layer;
		}
	}
	xenon::commandPos = cpos;
	while(layer){
		++xenon::commandPos;
		c = xenon::rawList.at(xenon::commandPos).first;
		if(c == "if" || c == "loop"){
			++layer;
		}else if(c == "end"){
			--layer;
		}
	}
	varManager.eraseLayer();
}

//stop execution
void xenonCommands::endall(ArgsIn_t argts){
	xenon::commandPos = INT16_MAX - 1;
}

//used for marking important parts of the code
void xenonCommands::tag(ArgsIn_t args){
	if(args.size() == 1){
		if(args.at(0) == "setloop"){
			xenon::defCommandPos = xenon::commandPos + 1;
			varManager.newLayer();
		}else if(args.at(0) == "endloop"){
			endall({});
		}
	}
}

//creates a new strip
void xenonCommands::newstrip(ArgsIn_t args){
	for(auto& s : xenon::stripList){
		if(args.at(0) == s->getName()){
			throw std::runtime_error("strip with same name");
		}else if(std::stoi(args.at(2)) == s->getPin()){
			throw std::runtime_error("strip with same pin");
		}
	}
	xenon::stripList.emplace_back(std::make_shared<xenon::Strip>(args.at(0), std::stoi(args.at(1)), std::stoi(args.at(2))));
}

//sets the current strip
void xenonCommands::setstrip(ArgsIn_t args){
	for(auto& s : xenon::stripList){
		if(args.at(0) == s->getName()){
			xenon::activeStrip = s;
			return;
		}
	}
	throw std::runtime_error("no strip found");
}
	
//update strip shown pixels
void xenonCommands::show(ArgsIn_t args){
	xenon::activeStrip->show();
}

//checker pattern in a single color
void xenonCommands::checker(ArgsIn_t args){
	for(int a = 0; a < xenon::activeStrip->getSize(); ++a){
		if(a % 2 == std::stoi(args.at(0))){
			xenon::activeStrip->set(a, std::stoi(args.at(1)), std::stoi(args.at(2)), std::stoi(args.at(3)));
		}
	}
}

std::string xenonTools::readf(std::ifstream& f){
	std::string s;
	char c;
	while(f.get(c)){
		s += c;
	}
	return s;
}

void xenonTools::process(const std::string& txt, xenon::CommandManager& comMan, std::vector<RawCommand_t>& raw){
	std::vector<RawCommand_t> cmdList;
	unsigned int line = 1;
	bool comment = false;
	bool paramfound = false;
	int layer = 0;
	std::string command;
	std::string arg;
	std::vector<std::string> args;
	bool inLoop = false;
	bool exitLoop = false;
	std::stack<int> ifStack;
	std::stack<int> loopStack;
	for(auto it = txt.begin(); it != txt.end(); ++it){
		char c = *it;
		if(c == '#'){
			comment = comment ? false : true;
			continue;
		}else if(c == '\n'){
			++line;
		}
		if(!comment){
			if(!paramfound){
				if((c == ' ' || c == '\t' || c == '\n')){
					if(!command.empty()){
						paramfound = true;
					}
				}else{
					command += c;
				}
			}if(paramfound){
				if(it + 1 == txt.end()){
					arg += c;
				}
				if(c == ' ' || c == '\t'){
					continue;
				}else if(c == '\n' || it + 1 == txt.end()){
					if(!arg.empty()){
						args.push_back(arg);
					}
					if(command == "if"){
						ifStack.push(layer);
						++layer;
					}else if(command == "else"){
						if(ifStack.top() != layer){
							throw xenon::XenonEx("error 10 line " + std::to_string(line) + "\nelse not after if");
						}
					}else if(command == "elseif"){
						if(ifStack.top() != layer){
							throw xenon::XenonEx("error 11 line " + std::to_string(line) + "\nelseif not after if");
						}
					}else if(command == "loop"){
						loopStack.push(layer);
						++layer;
					}else if(command == "end"){
						if(!ifStack.empty()){
							if(ifStack.top() == layer){
								ifStack.pop();
							}
						}else if(!loopStack.empty()){
							if(loopStack.top() == layer){
								loopStack.pop();
							}
						}
						--layer;
					}else if(command == "break" && !loopStack.empty()){
						throw xenon::XenonEx("error 3 line " + std::to_string(line) + "\nnot a valid command");
					}else if(command == "tag" && args.size() == 1){
						if(args.at(0) == "setloop"){
							inLoop = true;
						}else if(args.at(0) == "endloop"){
							exitLoop = true;
						}
					}
					if(comMan.find(command, args.size())){
						cmdList.emplace_back(command, args);
					}else if(!command.empty()){
						throw xenon::XenonEx("error 5 line " + std::to_string(line) + "\n" + command + " is not a valid command");
					}
					args.clear();
					arg.clear();
					command.clear();
					paramfound = false;
					continue;
				}else if(c == ','){
					args.push_back(arg);
					arg.clear();
					continue;
				}
				arg += c;
			}
		}else if(c == '\n'){
			++line;
		}
	}
	if(layer){
		throw xenon::XenonEx("error 4\nfinal layer depth not 0");
	}
	if(!inLoop){
		throw xenon::XenonEx("error 6\nno main loop start tag");
	}
	if(!exitLoop){
		throw xenon::XenonEx("error 7\nno main loop end tag");
	}
	raw = cmdList;
}

void xenonTools::fillVars(RawCommand_t& rawCommand, const xenon::CommandID& commandID){
	if(rawCommand.second.size() != commandID.argc){
		throw std::length_error("raw command args != command id getC()");
	}
	for(int a = 0; a < commandID.argc; ++a){
		try{
			xenon::Var* vu = xenonCommands::varManager.find(rawCommand.second.at(a));
			std::vector<std::string> tokens;
			size_t off = 0;
			std::list<xenon::Token> tl;
			switch(commandID.checklist.at(a)){
				case CHECKTYPE::CTnum:
					xenon::nlex(rawCommand.second.at(a), tl);
					if(tl.size() > 1){
						for(xenon::Token& t : tl){
							if(t.getType() == ETokenType::IDEN){
								xenon::Var* v = xenonCommands::varManager.find(t.getVal());
								if(v == nullptr){
									throw std::runtime_error("bad IDEN token");
								}
								switch(v->getType()){
									case VTYPE::Vint:
										t = xenon::Token(ETokenType::INT, std::to_string(v->getVal<int>()));
										break;
									case VTYPE::Vdouble:
										t = xenon::Token(ETokenType::DOUB, std::to_string(v->getVal<double>()));
										break;
								}
							}
						}
						rawCommand.second.at(a) = xenon::chew(tl).getVal();
					}
					if(vu){
						if(vu->getType() == VTYPE::Vint){
							rawCommand.second.at(a) = std::to_string(vu->getVal<int>());
							break;
						}else if(vu->getType() == VTYPE::Vdouble){
							rawCommand.second.at(a) = std::to_string(vu->getVal<double>());
							break;
						}
					}
					std::stod(rawCommand.second.at(a));
					break;
				case CHECKTYPE::CTvar:
					if(!vu){
						throw 0;
					}
					break;
				case CHECKTYPE::CTany:
					try{ std::stoi(rawCommand.second.at(a)); break; } catch(...){}
					try{ std::stod(rawCommand.second.at(a)); break; } catch(...){}
					if(vu){
						switch(vu->getType()){
							case VTYPE::Vint:
								rawCommand.second.at(a) = std::to_string(vu->getVal<int>());
								break;
							case VTYPE::Vdouble:
								rawCommand.second.at(a) = std::to_string(vu->getVal<double>());
								break;
						}
					}else{
						throw 0;
					}
					break;
			}
		}catch(...){
			throw std::invalid_argument("command arg not a valid type or var");
		}
	}
}

bool xenonTools::execute(std::vector<RawCommand_t>& commandList, xenon::CommandManager& cm){
	if(xenon::commandPos >= static_cast<int>(commandList.size())){
		xenon::commandPos = xenon::defCommandPos;
		xenonCommands::varManager.loopReset();
	}
	for(; xenon::commandPos < static_cast<int>(commandList.size()); ++xenon::commandPos){
		RawCommand_t pair = commandList.at(xenon::commandPos);
		bool fail = true;
		std::exception ex;
		for(const xenon::CommandID& c : cm.commands){
			if(c.name == pair.first && c.argc == pair.second.size()){
				try{
					xenonTools::fillVars(pair, c);
					c.func(pair.second);
					fail = false;
				}catch(std::exception e){
					ex = e;
				}
				break;
			}
		}
		if(fail){
			std::cout << ex.what() << std::endl;
			std::cout << "no command matching\nname: " << pair.first << "\nwith args: ";
			for(std::string& a: pair.second){
				std::cout << a << " , ";
			}
			std::cout << "\nwas found (make sure arguments are valid)\n";
			return false;
		}
		if(pair.first == "show"){
			++xenon::commandPos;
			break;
		}
	}
	return true;
}