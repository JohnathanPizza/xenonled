#include <xenonled/VarManager.hh>
#include <stack>
#include <vector>
#include <string>
#include <iostream>
#include <xenonled/essential.hh>
#include <xenonled/Var.hh>

xenon::VarManager::VarManager(){
	newLayer();
}

xenon::Var* xenon::VarManager::find(std::string s){
	for(Var& vp : vars){
		if(s == vp.getName()){
			return &vp;
		}
    }
	return nullptr;
}

void xenon::VarManager::add(const Var& vu){
	varLayers.top() += 1;
	vars.emplace_back(vu);
}

void xenon::VarManager::clear(){
	while(!varLayers.empty()){
		varLayers.pop();
	}
	vars.clear();
}

void xenon::VarManager::newLayer(){
	varLayers.push(0);
}

bool xenon::VarManager::eraseLayer(){
	if(varLayers.empty()){
		return false;
	}
	vars.erase(vars.end()- varLayers.top(), vars.end());
	varLayers.pop();
	return true;
}

void xenon::VarManager::loopReset(){
	while(varLayers.size() > 1){
		eraseLayer();
	}
	newLayer();
}

std::string xenon::VarManager::listall() const{
	std::string s;
	for(const Var& vu : vars){
		s += vu.getName() + " ";
	}
	return s;
}

size_t xenon::VarManager::getLayers(){
	return varLayers.size();
}

const std::vector<xenon::Var> xenon::VarManager::currentLayer() const{
	std::vector<Var> out;
	for(int a = 0; a < varLayers.top(); ++a){
		out.emplace_back(*(vars.rbegin() + a));
	}
	return out;
}