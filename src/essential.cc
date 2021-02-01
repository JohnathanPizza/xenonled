#include <xenonled/essential.hh>
#include <xenonled/command.hh>
#include <xenonled/VarManager.hh>

namespace xenon{
	std::shared_ptr<xenon::Strip> activeStrip = nullptr;
	int commandPos = 0;
	int defCommandPos = 0;
	xenon::CommandManager* mainCM = nullptr;
	std::vector<std::shared_ptr<xenon::Strip> > stripList;
	std::vector<RawCommand_t> rawList;
	std::stack<int> tempPos;
}

void xenon::reset(){
	defCommandPos = commandPos = 0;
	activeStrip = nullptr;
	stripList.clear();
	xenonCommands::varManager = VarManager();
}