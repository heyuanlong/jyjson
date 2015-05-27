#ifndef JYJSEXCEPT_HHH
#define JYJSEXCEPT_HHH
#include<exception>

class jsexception:public std::exception
{
public:
	std::string str;
	jsexception(std::string s):str(s){}
	~jsexception()throw(){};

	virtual const char *what() const throw(){return str.c_str();}

	/* data */
};

#endif

/*
exception
class exception {
public:
    exception() throw();
    exception(const exception& rhs) throw();
    exception& operator=(const exception& rhs) throw();
    virtual ~exception() throw();
    virtual const char *what() const throw();
    };

*/