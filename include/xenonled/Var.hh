#ifndef VAR_H
#define VAR_H

#include <string>

enum class VTYPE{Vint, Vdouble};

namespace xenon{
	class Var{
	private:
		union{
			int i;
			double d;
		};
		VTYPE type;
		std::string name;
	public:
		Var();
		~Var();
		Var(std::string n);
		Var(const Var& u);
		Var& operator=(const Var& v);
		std::string getName() const;
		void set(int v);
		void set(double v);
		VTYPE getType() const;
		template<typename T>
		T& getVal();
	};
}

#endif