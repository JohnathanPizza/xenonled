#include <xenonled/argManip.hh>
#include <string>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <array>
#include <algorithm>
#include <stdexcept>
#include <iterator>

xenon::Token::Token() : value(""), type(ETokenType::INT){}

xenon::Token::Token(ETokenType t, std::string v) : value(v), type(t){}

ETokenType xenon::Token::getType() const{
	return type;
}

std::string xenon::Token::getVal() const{
	return value;
}

std::string xenon::Token::str() const{
	char out[100];
	if(op()){
		sprintf_s(out, "type:%i", type);
	}else{
		sprintf_s(out, "type:%i val:%s", type, value.c_str());
	}
	return std::string().assign(out);
}

bool xenon::Token::op() const{
	return type != ETokenType::INT && type != ETokenType::DOUB && type != ETokenType::IDEN;
}

bool xenon::Token::brack() const{
	return type == ETokenType::OPAR || type == ETokenType::CPAR;
}

xenon::Token xenon::classify(const std::string& s){
	std::string nums("0123456789");
	std::array<std::string, 20> patterns{"+", "-", "*", "/", "%", "==", "!=", "(", ")", "<", ">", "&", "|", "^", "||", "&&", "<<", ">>", "<=", ">="};
	auto found = std::find(patterns.begin(), patterns.end(), s);
	if(found != patterns.end()){
		return static_cast<ETokenType>(std::distance(patterns.begin(), found));
	}else if(nums.find(s.at(0)) != std::string::npos){
		bool dec = false;
		for(const char& c : s){
			if(c == '.'){
				if(dec){
					throw std::runtime_error("cant have 2 decimals");
				}
				dec = true;
			}else if(nums.find(c) == std::string::npos){
				throw std::runtime_error("not a valid number literal " + s);
			}
		}
		if(dec){
			return xenon::Token(ETokenType::DOUB, s);
		}else{
			return xenon::Token(ETokenType::INT, s);
		}
	}else{
		return xenon::Token(ETokenType::IDEN, s);
	}
}

void xenon::nlex(const std::string& in, std::list<Token>& o){
	o.clear();
	std::string segment;
	std::string tokens("()+-*/%=!<>&|^");
	for(auto it = in.begin(); it != in.end(); ++it){
		char c = *it;
		if(c != '\n' && it != in.end() - 1){
			if(c != '\t' && c != ' '){
				segment += c;
			}
		}else if(!segment.empty()){
			if(it == in.end() - 1){
				segment += c;
			}
			std::string toInsert;
			for(auto segit = segment.begin(); segit != segment.end(); ++segit){
				if(tokens.find_first_of(*segit) != std::string::npos){
					if(!toInsert.empty()){
						o.emplace_back(classify(toInsert));
						toInsert.clear();
					}
					if(*segit == '&' || *segit == '|' || *segit == '<' || *segit == '>' || *segit == '=' || *segit == '!'){
						if(*segit == *(segit + 1)){
							toInsert = std::string(2, *segit);
							++segit;
						}else if((*segit == '<' || *segit == '>' || *segit == '!') && *(segit + 1) == '='){
							toInsert = std::string() + *segit + '=';
							++segit;
						}else{
							toInsert = std::string(1, *segit);
						}
					}else{
						toInsert = std::string(1, *segit);
					}
					o.emplace_back(classify(toInsert));
					toInsert.clear();
				}else{
					toInsert += *segit;
				}
			}
			if(!toInsert.empty()){
				o.emplace_back(classify(toInsert));
			}
			segment.clear();
		}
	}
}

std::list<xenon::Token> xenon::subchew(std::list<Token> toks, const std::set<ETokenType>& tl){
	using namespace xenon;
	for(auto it = toks.begin(); it != toks.end(); ++it){
		auto i = tl.find(it->getType());
		if(i != tl.end()){
			ETokenType currentType = *i;
			advance(it, -1);
			Token left = *it;
			advance(it, 2);
			Token right = *it;
			if(left.op() || right.op()){
				throw std::runtime_error("bad subchew");
			}
			advance(it, -1);
			Token inserting;
			switch(currentType){
				case ETokenType::MULT:
					if(left.getType() == ETokenType::INT && right.getType() == ETokenType::INT){
						inserting = Token(ETokenType::INT, std::to_string(stoi(left.getVal()) * stoi(right.getVal())));
					}else{
						inserting = Token(ETokenType::DOUB, std::to_string(stod(left.getVal()) * stod(right.getVal())));
					}
					break;
				case ETokenType::DIV:
					if(left.getType() == ETokenType::INT && right.getType() == ETokenType::INT){
						inserting = Token(ETokenType::INT, std::to_string(stoi(left.getVal()) / stoi(right.getVal())));
					}else{
						inserting = Token(ETokenType::DOUB, std::to_string(stod(left.getVal()) / stod(right.getVal())));
					}
					break;
				case ETokenType::ADD:
					if(left.getType() == ETokenType::INT && right.getType() == ETokenType::INT){
						inserting = Token(ETokenType::INT, std::to_string(stoi(left.getVal()) + stoi(right.getVal())));
					}else{
						inserting = Token(ETokenType::DOUB, std::to_string(stod(left.getVal()) + stod(right.getVal())));
					}
					break;
				case ETokenType::SUB:
					if(left.getType() == ETokenType::INT && right.getType() == ETokenType::INT){
						inserting = Token(ETokenType::INT, std::to_string(stoi(left.getVal()) - stoi(right.getVal())));
					}else{
						inserting = Token(ETokenType::DOUB, std::to_string(stod(left.getVal()) - stod(right.getVal())));
					}
					break;
				case ETokenType::MOD:
					inserting = Token(ETokenType::INT, std::to_string(stoi(left.getVal()) % stoi(right.getVal())));
					break;
				case ETokenType::AND:
					inserting = Token(ETokenType::INT, std::to_string(stod(left.getVal()) && stod(right.getVal())));
					break;
				case ETokenType::OR:
					inserting = Token(ETokenType::INT, std::to_string(stod(left.getVal()) || stod(right.getVal())));
					break;
				case ETokenType::GREATER:
					inserting = Token(ETokenType::INT, std::to_string(stod(left.getVal()) > stod(right.getVal())));
					break;
				case ETokenType::LESS:
					inserting = Token(ETokenType::INT, std::to_string(stod(left.getVal()) < stod(right.getVal())));
					break;
				case ETokenType::NOT:
					inserting = Token(ETokenType::INT, std::to_string(left.getVal() != right.getVal()));
					break;
				case ETokenType::EQ:
					inserting = Token(ETokenType::INT, std::to_string(left.getVal() == right.getVal()));
					break;
				case ETokenType::BXOR:
					inserting = Token(ETokenType::INT, std::to_string(stoi(left.getVal()) ^ stoi(right.getVal())));
					break;
				case ETokenType::BOR:
					inserting = Token(ETokenType::INT, std::to_string(stoi(left.getVal()) | stoi(right.getVal())));
					break;
				case ETokenType::BAND:
					inserting = Token(ETokenType::INT, std::to_string(stoi(left.getVal()) & stoi(right.getVal())));
					break;
				case ETokenType::BSL:
					inserting = Token(ETokenType::INT, std::to_string(stoi(left.getVal()) << stoi(right.getVal())));
					break;
				case ETokenType::BSR:
					inserting = Token(ETokenType::INT, std::to_string(stoi(left.getVal()) >> stoi(right.getVal())));
					break;
				case ETokenType::LESSEQ:
					inserting = Token(ETokenType::INT, std::to_string(stod(left.getVal()) <= stod(right.getVal())));
					break;
				case ETokenType::GREATEREQ:
					inserting = Token(ETokenType::INT, std::to_string(stod(left.getVal()) >= stod(right.getVal())));
					break;
			}
			advance(it, -1);
			toks.insert(it, inserting);
			auto fit = it;
			advance(it, 3);
			toks.erase(fit, it);
			it = toks.begin();
		}
	}
	return toks;
}

xenon::Token xenon::chew(std::list<Token> toks){
	std::vector<std::set<ETokenType> > typeseq{
		{ETokenType::MULT, ETokenType::DIV, ETokenType::MOD},
		{ETokenType::ADD, ETokenType::SUB},
		{ETokenType::BSL, ETokenType::BSR},
		{ETokenType::LESS, ETokenType::GREATER, ETokenType::LESSEQ, ETokenType::GREATEREQ},
		{ETokenType::EQ, ETokenType::NOT},
		{ETokenType::BAND},
		{ETokenType::BXOR},
		{ETokenType::BOR},
		{ETokenType::AND},
		{ETokenType::OR}
	};
	bool pass = false;
	while(!pass){
		pass = true;
		std::list<Token>::iterator parenth;
		for(auto it = toks.begin(); it != toks.end(); ++it){
			if(it->getType() == ETokenType::OPAR){
				pass = false;
				parenth = it;
			}
		}
		if(!pass){
			std::list<Token>::iterator endit;
			int size = -1;
			for(auto it = parenth; it != toks.end(); ++it){
				if(it->getType() == ETokenType::CPAR){
					endit = it;
					break;
				}else{
					++size;
				}
			}
			std::list<Token> sub(size);
			advance(parenth, 1);
			copy(parenth, endit, sub.begin());
			for(std::set<ETokenType> tl : typeseq){
				sub = xenon::subchew(sub, tl);
			}
			advance(parenth, -1);
			toks.emplace(parenth, sub.front());
			advance(endit, 1);
			toks.erase(parenth, endit);
		}
	}
	std::list<Token> fin = toks;
	for(std::set<ETokenType> tl : typeseq){
		fin = xenon::subchew(fin, tl);
	}
	return fin.front();
}