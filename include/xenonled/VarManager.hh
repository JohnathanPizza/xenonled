#ifndef VAR_MANAGER_H
#define VAR_MANAGER_H

#include <stack>
#include <vector>
#include <string>
#include <xenonled/Var.hh>

//used to manage variable lifetimes
namespace xenon{
	class VarManager{
		std::stack<int> varLayers;
		std::vector<Var> vars;
	public:
		VarManager();
		Var* find(std::string s);
		void add(const Var& vu);
		void newLayer();
		bool eraseLayer();
		void loopReset();
		void clear();
		std::string listall() const;
		size_t getLayers();
		const std::vector<Var> currentLayer() const;
	};
}

#endif