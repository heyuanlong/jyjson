#ifndef JYVALUE_HHH
#define JYVALUE_HHH


#include <iostream>
#include <map>
#include <vector>
#include <string>

//#define DEBUG ok

#ifdef DEBUG
	#define COUTDEBUG std::cout<<"("<<__FILE__<<":"<<__LINE__<<")"<<std::endl;
#else
	#define COUTDEBUG
#endif

enum jyjson_shuzhi{ jyjson_numint, jyjson_numdouble, jyjson_string, jyjson_object, jyjson_array, jyjson_bool, jyjson_null, jyjson_nothing };
//string  int double  object array bool null nothing 

class ValueObj;
class ValueArr;

class Value
{
public:
	Value(){};
	~Value(){};

protected:
	bool clean(int a, void*p);//清理
	void* copynew(int a,void* p);
};

struct TypeValue:public Value
{
	int type;
	void *vpoint;///这里的指针会放到其他类中的容器里
	TypeValue(int a,void *p):type(a),vpoint(p){}
	~TypeValue(){};

	bool isint()const;//判断类型
	bool isdouble()const;
	bool isstring()const;
	bool isobject()const;
	bool isarray()const;
	bool isbool()const;
	bool isnull()const;
	bool isnothing()const;

	int getint()const;//获取对应的值
	double getdouble()const;
	std::string getstring()const;
	ValueObj* getpointobject()const;
	ValueArr* getpointarray()const;
	ValueObj getobject()const;
	ValueArr getarray()const;	
	bool getbool()const;
	void* getnull()const;

	operator int()const;
    operator double()const;
    operator std::string()const;
    operator ValueObj*()const;
	operator ValueArr*()const;
    operator ValueObj()const;
	operator ValueArr()const;	
    operator bool()const;
    operator void*()const;

    TypeValue& operator=(int);//obj["key"]=1
    TypeValue& operator=(double);
    TypeValue& operator=(std::string);
    TypeValue& operator=(char *);
	TypeValue& operator=(const ValueObj &);
	TypeValue& operator=(const ValueArr &);
    TypeValue& operator=(bool);
    
    TypeValue& setnull(TypeValue&);//
    TypeValue& setnothing(TypeValue&);//

    TypeValue& operator [](int);
    TypeValue& operator [](std::string);
    TypeValue& operator [](const char *);

    bool clean();//
private:
	bool clean(int a, void*p);//清理

};

class ValueObj:public Value
{
public:
	std::map<std::string ,TypeValue> mypair;
	int a;
	ValueObj(){};
	~ValueObj();
	ValueObj(const ValueObj &org);//禁止复制和赋值
	ValueObj& operator =(const ValueObj &org);

    bool insert(std::string str,int type,void *vpoint);
    TypeValue& operator [](std::string str);

    bool havekey(std::string str);//有无这个key
    bool clean(std::string str);//清理单个mypair   
    bool clean();//清理整个ValueObj对象
private:
	bool clean(int a, void*p);//清理

};

class ValueArr:public Value
{
public:
	ValueArr(){};
	~ValueArr();
	ValueArr(const ValueArr &org);//禁止复制和赋值
	ValueArr& operator =(const ValueArr &org);

	std::vector<TypeValue> vec;
	bool insert(int type,void *vpoint);
	TypeValue& operator [](int i);
	int size();

    bool clean(int i);//清理单个array
    bool clean();//清理整个valueArr对象

private:
	bool clean(int a, void*p);//清理

};
#endif

