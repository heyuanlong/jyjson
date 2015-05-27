#ifndef JYWRITE_HHH
#define JYWRITE_HHH

#include "value.h"

class Write
{
public:
	Write(){};
	~Write(){};

	void run(ValueObj &vobj ,std::string &str)//对象到json
	{
		str = ""; 
		run1(vobj, str);

		if (str.length()==1)
		 str=""; 
	}
	void run(ValueArr &varr ,std::string &str)//数组到json
	{
		str = ""; 
		run1(varr, str);
		
		if (str.length()==1)
		  str="";
	}
private:
	void run1(ValueObj& ,std::string &);//对象到json
	void run1(ValueArr& ,std::string &);//数组到json

	void select_type(int a, void*p, std::string &str);
	void get_string(void*p,std::string &str);
	void get_int(void*p,std::string &str);
	void get_double(void*p,std::string &str);
	void get_object(void*p,std::string &str);
	void get_array(void*p,std::string &str);
	void get_bool(void*p,std::string &str);
	void get_null(void*p,std::string &str);
	//void Write::myitoa(int a , char *str);
};

#endif

/*
{
"a":5000.119999999999890860,
"b":a' g,f'd[][{{}{}'f,
"c":[ccc,123],
"d":[[{"shendu":[{"ceshi":1},shendu]}]],
"e":fgfdgfd,
"sh":[[{"shendu":[{"ceshi":yes,googogo}]}]
]}

{"a":5000.119999999999890860,
"b":"a' g,f'd[][{{}{}'f",
"c":["ccc",123],
"d":[   [   {"shendu":  [{"ceshi":1},"shendu"]  }]
"e":"fgfdgfd",
"sh":[[{"shendu":[{"ceshi":"yes,googogo"}]}]] }


{"a":5000.12,
"b":"a' g,f'd[][{\{}{}'f",
"c":["d,ed",2,{"vv":true},[1,2],true,null],"d":{"ss":"sisi"}}


{
  "a":1,
  "b":"a' g,f'd[][{{}{}'f",
  "c":["d,ed",2,{"vv":1},[1,2],true,null],
  "d":{"ss":1}
}
*/
