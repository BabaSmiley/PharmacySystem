#include <sstream>

struct stringbuilder
{
	std::stringstream ss;
	template<typename T>
	stringbuilder & operator << (const T &data)
	{
		ss << data;
		return *this;
	}
	operator std::string() { return ss.str(); }
};