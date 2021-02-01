#ifndef ARGMAN_H
#define ARGMAN_H

#include <vector>
#include <list>
#include <set>
#include <string>

enum class ETokenType{
	ADD,
	SUB,
	MULT,
	DIV,
	MOD,
	EQ,
	NOT,
	OPAR,
	CPAR,
	LESS,
	GREATER,
	BAND,
	BOR,
	BXOR,
	OR,
	AND,
	BSL,
	BSR,
	LESSEQ,
	GREATEREQ,
	INT,
	DOUB,
	IDEN
};

namespace xenon{
	class Token{
		std::string value;
		ETokenType type;
	public:
		Token();
		Token(ETokenType t, std::string v = "");
		ETokenType getType() const;
		std::string getVal() const;
		std::string str() const;
		bool op() const;
		bool brack() const;
	};
	
	Token classify(const std::string& s);
	void nlex(const std::string& in, std::list<Token>& out);
	std::list<Token> subchew(std::list<Token> toks, const std::set<ETokenType>& tl);
	Token chew(std::list<Token> toks);
}

#endif