#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
#include <iostream>
#include <stack>
#include <fstream>
#include <xenonled/VarManager.hh>
#include <xenonled/CommandID.hh>
#include <xenonled/essential.hh>
#include <xenonled/argManip.hh>
#include <xenonled/CommandManager.hh>

namespace xenonCommands{
	extern xenon::VarManager varManager;
	//placeholder
	void none(ArgsIn_t);
	//changes a variable's value
	void set(ArgsIn_t args);
	//sets a pixel on active strip to an rgb value
	void setpix(ArgsIn_t args);
	//create a new int variable
	void i(ArgsIn_t args);
	//create a new double variable
	void d(ArgsIn_t args);
	//executes code up to next endif on same layer if var != 0
	void _if(ArgsIn_t args);
	//else switch for if statement
	void _else(ArgsIn_t args);
	//elseif switch for if statement
	void elseif(ArgsIn_t args);
	//loops code until next endloop on same layer until var == 0
	void loop(ArgsIn_t args);
	//marks end of loops and ifs
	void end(ArgsIn_t args);
	//stop execution
	void endall(ArgsIn_t args);
	//swaps two pixel values in active strip
	void swapd(ArgsIn_t args);
	//sets all pixels in active strip to a number/pixel value
	void setall(ArgsIn_t args);
	//breaks a loop
	void _break(ArgsIn_t args);
	//used for marking important parts of the code
	void tag(ArgsIn_t args);
	//creates a new strip
	void newstrip(ArgsIn_t args);
	//sets the current strip
	void setstrip(ArgsIn_t args);
	//update strip shown pixels
	void show(ArgsIn_t args);
	//checker pattern in a single color
	void checker(ArgsIn_t args);
}

namespace xenonTools{
	std::string readf(std::ifstream& f);
	void process(const std::string& txt, xenon::CommandManager& comMan, std::vector<RawCommand_t>& raw);
	void fillVars(RawCommand_t& rawCommand, const xenon::CommandID& commandIDList);
	bool execute(std::vector<RawCommand_t>& commandList, xenon::CommandManager& cm);
}

#endif
