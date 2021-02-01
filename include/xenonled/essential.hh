#ifndef ESSENTIAL_H
#define ESSENTIAL_H

#include <vector>
#include <memory>
#include <xenonled/Strip.hh>
#include <xenonled/Pixel.hh>
#include <xenonled/CommandManager.hh>
#include <stack>

using RawCommand_t = std::pair<std::string, std::vector<std::string> >;

namespace xenon{
	extern std::vector<std::shared_ptr<xenon::Strip> > stripList;
	extern std::shared_ptr<xenon::Strip> activeStrip;
	extern std::vector<RawCommand_t> rawList;
	extern int commandPos;
	extern int defCommandPos;
	extern std::stack<int> tempPos;
	extern xenon::CommandManager* mainCM;
	void reset();
}

#endif