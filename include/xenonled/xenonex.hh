#ifndef XENONEX_H
#define XENONEX_H

#include <exception>
#include <string>

namespace xenon{
	class XenonEx : public std::exception{
	protected:
		std::string val;
	public:
		XenonEx();
		XenonEx(const std::string& s);
		virtual ~XenonEx(){}
		virtual const char* what() const noexcept;
		XenonEx& operator=(const std::exception& e);
	};
}

#endif