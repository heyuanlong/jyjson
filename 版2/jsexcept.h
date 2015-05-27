#ifndef JYJSEXCEPT_HHH
#define JYJSEXCEPT_HHH
#include<exception>

class jsexception:public std::exception
{
public:
	std::string str;
	jsexception(std::string s):str(s){}
	~jsexception()throw(){};

	virtual const char *what() const throw()
	{return str.c_str();}
};

#endif

