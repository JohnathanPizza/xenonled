#include <xenonled/CommandManager.hh>
#include <xenonled/CommandID.hh>
#include <xenonled/command.hh>

bool xenon::CommandManager::find(std::string name, int32_t argc){
	for(const CommandID& cs : commands){
		if(cs.name == name && cs.argc == argc){
			return true;
		}
	}
	return false;
}