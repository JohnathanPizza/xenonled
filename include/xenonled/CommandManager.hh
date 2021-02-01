#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H

#include <vector>
#include <string>
#include <xenonled/CommandID.hh>

namespace xenon{
	struct CommandManager{
		std::vector<xenon::CommandID> commands;
		bool find(std::string name, int32_t argc);
	};
}

#endif