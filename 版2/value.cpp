#include "value.h"
#include <string>
#include <utility>
#include "jsexcept.h"

bool Value::clean(int a,void*p)//清理
{
	switch(a)
		{
			case jyjson_string:
				delete (std::string*)p;
			break;
			case jyjson_numint:
				delete (int*)p;
			break;
			case jyjson_numdouble:
				delete (double*)p;
			break;
			case jyjson_object:
				delete (ValueObj*)p;
			break;
			case jyjson_array:
				delete (ValueArr*)p;
			break;
			case jyjson_bool:
				delete (bool*)p;
			break;
			case jyjson_null:
			break;
			case jyjson_nothing:
			break;
		}
	return true;
}
void* Value::copynew(int a,void* p)//用于复制控制
{
		switch(a)
		{
			case jyjson_string:
				return new std::string(*(std::string*)p);
			break;
			case jyjson_numint:
				return new int(*(int*)p);
			break;
			case jyjson_numdouble:
				return new double(*(double*)p);
			break;
			case jyjson_object:
				return new ValueObj(*(ValueObj*)p);
			break;
			case jyjson_array:
				return new ValueArr(*(ValueArr*)p);
			break;
			case jyjson_bool:
				return new bool((bool*)p);
			break;
			case jyjson_null:
			break;
			case jyjson_nothing:
			break;
		}
}


ValueObj::~ValueObj()
{
	clean();//清理整个对象
}
bool ValueObj::insert(std::string str,int type,void *vpoint)
{

	std::map<std::string ,TypeValue>::iterator f = mypair.find(str);
	if (f != mypair.end())//存在
	{
		int kk = f->second.type;
		void *pp = f->second.vpoint;
		clean(f->second.type,f->second.vpoint);
		f->second=TypeValue(type,vpoint);//改变原来的值
		return true;
	}
	else{//不存在
		std::pair<std::map<std::string ,TypeValue>::iterator,bool> ret;
		ret=mypair.insert(make_pair(str,TypeValue(type,vpoint)));
		if (ret.second)
			return true;
		else
			return false;
	}
}
TypeValue& ValueObj::operator[](std::string str)
{
	std::map<std::string, TypeValue>::iterator f = mypair.find(str);
	if (f != mypair.end())//存在
		return f->second;

	else//不存在
	{
		std::pair<std::map<std::string, TypeValue>::iterator, bool> ret;
		ret=mypair.insert(make_pair(str,TypeValue(jyjson_nothing,NULL)));

		if (ret.second)
			return ret.first->second;
		else
			throw jsexception("key is not exist,so excute insert ,but insert was fail");
	}
}
bool ValueObj::havekey(std::string str)//有无这个key
{
	std::map<std::string ,TypeValue>::iterator f = mypair.find(str);

	if (f != mypair.end())//存在
		return true;
	else//不存在
		return false;
}
bool ValueObj::clean(std::string str)//清理单个mypair
{
	std::map<std::string ,TypeValue>::iterator f = mypair.find(str);
	if (f != mypair.end())
	{
	    clean(f->second.type,f->second.vpoint);
		mypair.erase(str);
		return true;
	}
	return false;
}
bool ValueObj::clean()//清理整个ValueObj对象
{
	COUTDEBUG
	std::map<std::string ,TypeValue>::iterator b=mypair.begin();
	std::map<std::string ,TypeValue>::iterator e=mypair.end();

	for (; b != e; ++b)
		b->second.clean();

	mypair.clear();//清空这个map容器
	return true;
}
bool ValueObj::clean(int a, void*p)//清理
{
	Value::clean(a, p);
	return true;
}

ValueObj::ValueObj(const ValueObj &org)//复制和赋值
{
	std::map<std::string, TypeValue>::const_iterator b = org.mypair.begin();
	std::map<std::string, TypeValue>::const_iterator e = org.mypair.end();

void *vp;	
std::pair<std::map<std::string ,TypeValue>::iterator,bool> ret;
	for (; b != e; ++b)
	{
		vp=copynew(b->second.type,b->second.vpoint);	
		ret=mypair.insert(make_pair(b->first,TypeValue(b->second.type,vp)));

		if (!ret.second)
			throw jsexception("have error in ValueObj construct");
	}		
}

ValueObj& ValueObj::operator =(const ValueObj &org)//复制和赋值
{
	if (&org != this)
	{
		std::map<std::string, TypeValue>::const_iterator b = org.mypair.begin();
		std::map<std::string, TypeValue>::const_iterator e = org.mypair.end();

		void *vp;	
		std::pair<std::map<std::string ,TypeValue>::iterator,bool> ret;
		for (; b != e; ++b)
		{
			vp=copynew(b->second.type,b->second.vpoint);		
			ret=mypair.insert(make_pair(b->first,TypeValue(b->second.type,vp)));

			if (!ret.second)
				throw jsexception("have error in ValueObj construct");
		}
	}
	return *this;
}


bool ValueArr::insert(int type,void *vpoint)
{
    vec.push_back(TypeValue(type,vpoint));
    return true;
}

TypeValue& ValueArr::operator [](int i)
{
	if (i>= vec.size() || i<0)
		throw jsexception("index is illegal");

    return vec[i];
}
int ValueArr::size()
{
	return vec.size();
}
bool ValueArr::clean(int i)//清理单个array
{
	if (i> vec.size() || i<0)
		throw jsexception("index is illegal");

    std::vector<TypeValue>::iterator b= vec.begin();
    b+=i;
    clean(b->type,b->vpoint);

    vec.erase(b);
    return true;
}

bool ValueArr::clean()//清理整个valueArr对象
{
	COUTDEBUG
	std::vector<TypeValue>::iterator b = vec.begin();
	std::vector<TypeValue>::iterator e = vec.end();
	for (; b != e; ++b)
		clean(b->type,b->vpoint);
	vec.clear();
	return true;
}
bool ValueArr::clean(int a, void*p)//清理
{
		Value::clean(a, p);
		return true;
}

ValueArr::~ValueArr()
{
    clean();
}

ValueArr::ValueArr(const ValueArr &org)//复制和赋值
{
	std::vector<TypeValue>::const_iterator b = org.vec.begin();
	std::vector<TypeValue>::const_iterator e = org.vec.end();

	void *vp;
	for (; b != e; ++b)
		{
			vp=copynew(b->type,b->vpoint);
			vec.push_back(TypeValue(b->type,vp));
		}
}
ValueArr& ValueArr::operator =(const ValueArr &org)//复制和赋值
{
	if (&org != this)
	{
		std::vector<TypeValue>::const_iterator b = org.vec.begin();
		std::vector<TypeValue>::const_iterator e = org.vec.end();

		void *vp;
		for (; b != e; ++b)
			{
				vp=copynew(b->type,b->vpoint);
				vec.push_back(TypeValue(b->type,vp));
			}
	}
	return *this;
}


bool TypeValue::isint()const//判断类型
{	return type==jyjson_numint;     }

bool TypeValue::isdouble()const
{	return type==jyjson_numdouble;     }

bool TypeValue::isstring()const
{	return type==jyjson_string;     }

bool TypeValue::isobject()const
{	return type==jyjson_object;     }

bool TypeValue::isarray()const
{	return type==jyjson_array;     }

bool TypeValue::isbool()const
{	return type==jyjson_bool;     }

bool TypeValue::isnull()const
{	return type==jyjson_null;     }

bool TypeValue::isnothing()const
{	return type==jyjson_nothing;     }


int TypeValue::getint()const//获取对应的值
{   //int可以给double
	if (type!=jyjson_numint && type!=jyjson_numdouble)
		throw jsexception("type is not match");

	return *( (int*)vpoint );
}
double TypeValue::getdouble()const
{   //doubel不能赋值给int
	if (type!=jyjson_numdouble)
		throw jsexception("type is not match");

	return *( (double*)vpoint );
}
std::string TypeValue::getstring()const
{	
	if (type!=jyjson_string)
		throw jsexception("type is not match");

	return *((std::string*)vpoint);
}

ValueObj* TypeValue::getpointobject()const
{	
	if (type!=jyjson_object)
		throw jsexception("type is not match");
	
	return (ValueObj*)vpoint;
}
ValueArr* TypeValue::getpointarray()const
{	
	if (type!=jyjson_array)
		throw jsexception("type is not match");
	
	return (ValueArr*)vpoint;
}

ValueObj TypeValue::getobject()const
{	
	if (type!=jyjson_object)
		throw jsexception("type is not match");
	
	return (*(ValueObj*)vpoint);//会调用ValueObj拷贝构造函数
}
ValueArr TypeValue::getarray()const
{	
	if (type!=jyjson_array)
		throw jsexception("type is not match");
	
	return (*(ValueArr*)vpoint);//会调用ValueArr拷贝构造函数
}

bool TypeValue::getbool()const
{	
	if (type!=jyjson_bool)
		throw jsexception("type is not match");
	
	return *( (bool*)vpoint );
}
void* TypeValue::getnull()const
{	
	if (type!=jyjson_null)
		throw jsexception("type is not match");
	
	return (void*)vpoint;
}

TypeValue::operator int()const
{ return getint();  }

TypeValue::operator double()const
{ return getdouble();}

TypeValue::operator std::string()const
{return getstring();}

TypeValue::operator ValueObj*()const
{ return getpointobject(); }

TypeValue::operator ValueArr*()const
{ return getpointarray(); }

TypeValue::operator ValueObj()const
{ return getobject(); }

TypeValue::operator ValueArr()const
{ return getarray(); }

TypeValue::operator bool()const
{ return getbool(); }
TypeValue::operator void*()const
{ return getnull(); }


TypeValue& TypeValue::operator=(int a)//obj["key"]=1
{
   clean(this->type,this->vpoint);
   this->type=jyjson_numint;
   this->vpoint=new int(a);
   return *this;
}
TypeValue& TypeValue::operator=(double a)
{
	clean(this->type,this->vpoint);
   this->type=jyjson_numdouble;
   this->vpoint=new double(a);
   return *this;
}
TypeValue& TypeValue::operator=(std::string a)
{
	clean(this->type,this->vpoint);
   this->type=jyjson_string;
   this->vpoint=new std::string(a);
   return *this;
}
TypeValue& TypeValue::operator=(char *a)
{
	clean(this->type,this->vpoint);
   this->type=jyjson_string;
   this->vpoint=new std::string(a);
   return *this;
}

TypeValue& TypeValue::operator=(const ValueObj &org)
{
   clean(this->type,this->vpoint);
   this->type=jyjson_object;
   this->vpoint=new ValueObj(org);
   return *this;
}  
TypeValue& TypeValue::operator=(const ValueArr &org)
{
	clean(this->type,this->vpoint);
   this->type=jyjson_array;
   this->vpoint=new ValueArr(org);
   return *this;
}
TypeValue& TypeValue::operator=(bool a)
{
	clean(this->type,this->vpoint);
   this->type=jyjson_bool;
   this->vpoint=new bool(a);
   return *this;
}

TypeValue& TypeValue::setnull(TypeValue& a)//only NULL
{
   a.clean(this->type,this->vpoint);
   a.type=jyjson_null;
   a.vpoint=NULL;
   return a;
}
TypeValue& TypeValue::setnothing(TypeValue& a)//only NULL
{
   a.clean(this->type,this->vpoint);
   a.type=jyjson_nothing;
   a.vpoint=NULL;
   return a;
}

TypeValue& TypeValue::operator [](int index)
{
	if (isarray())//本来就是数组
	{
		ValueArr* p=(ValueArr*)this->vpoint;
		int si = p->size();
		if (index < 0 || index > si )
		{
			throw jsexception("index error");
		}

		if (index == p->size())//插入新的一个
		{
			p->vec.push_back(TypeValue(jyjson_nothing,NULL));

		}
		return p->vec.at(index);
	}
	else//本来不是数组，所以要以开始
	{
		if (index != 0)//新数组必须从0开始
			throw jsexception("index error");
		
		clean();
		this->type=jyjson_array;
		this->vpoint=new ValueArr;//调试的时候发现这里，错写成ValueObj
		ValueArr* p2=(ValueArr*)this->vpoint;
		p2->insert(jyjson_nothing,NULL);
		return p2->operator [](index);
	}
}
TypeValue& TypeValue::operator [](std::string str)
{
	if (isobject())//本来就是对象
	{
		ValueObj* p=(ValueObj*)this->vpoint;
		return p->operator [](str);
	}
	else//本来不是对象
	{
		clean();
		this->type=jyjson_object;
		this->vpoint=new ValueObj;
		ValueObj* p2=(ValueObj*)this->vpoint;

		return p2->operator [](str);
	}
}
TypeValue& TypeValue::operator [](const char *str)
{
	std::string ss(str);
	return operator [](ss);
}
bool TypeValue::clean()//清理整个valueArr对象
{
  clean(this->type,this->vpoint);
   this->type=jyjson_nothing;
   this->vpoint=NULL;
   return true;
}
bool TypeValue::clean(int a, void*p)//清理
{
	Value::clean(a, p);
	return true;
}

