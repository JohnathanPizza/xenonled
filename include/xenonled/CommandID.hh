#ifndef COMMAND_ID_H
#define COMMAND_ID_H

#include <string>
#include <vector>

using ArgsIn_t = std::vector<std::string>;

enum class CHECKTYPE{CTnum, CTvoid, CTvar, CTany};

namespace xenon{
	//used for organizing commands with arg count, type, name, and function pointer for func in Commands.hh
	struct CommandID{
		const std::string name;
		const int argc;
		std::vector<CHECKTYPE> checklist;
		void (*const func)(ArgsIn_t);
		CommandID();
		template<typename... A>
		CommandID(std::string n, void(*f)(std::vector<std::string>), A... list) : name(n), func(f), checklist({list...}), argc(sizeof...(A)){}
	};
}

#endif