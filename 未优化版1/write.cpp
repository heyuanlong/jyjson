#include "write.h"
#include <cstdlib>
#include <sstream>



void Write::run1(ValueObj &vobj,std::string &str)//对象到json
{
	COUTDEBUG
	std::map<std::string ,TypeValue>::iterator b=vobj.mypair.begin();
	std::map<std::string ,TypeValue>::iterator e=vobj.mypair.end();
	str.append("{");
	for (; b != e; ++b)
	{
		str+="\""+b->first+"\":";
		select_type(b->second.type,b->second.vpoint,str);
		str.append(",");
	}

	str[str.length()-1]='}';//覆盖最后一个多出来的","
}
void Write::run1(ValueArr& varr,std::string &str)//数组到json
{
	COUTDEBUG
	std::vector<TypeValue>::iterator b = varr.vec.begin();
	std::vector<TypeValue>::iterator e = varr.vec.end();
	str.append("[");
	for (; b != e; ++b)
	{

		select_type(b->type,b->vpoint,str);
		str.append(",");
	}

	str[str.length()-1]=']';//覆盖最后一个多出来的","
}
void Write::select_type(int a, void*p, std::string &str)
{
	switch(a)
		{
			case jyjson_string:
				get_string(p,str);
			break;
			case jyjson_numint:
				get_int(p,str);
			break;
			case jyjson_numdouble:
				get_double(p,str);
			break;
			case jyjson_object:
				get_object(p,str);
			break;
			case jyjson_array:
				get_array(p,str);
			break;
			case jyjson_bool:
				get_bool(p,str);
			break;
			case jyjson_null:
				get_null(p,str);
			break;
			case jyjson_nothing:
			break;
		}
}
void Write::get_string(void*p,std::string &str)
{

	str+="\""+*( (std::string*)p )+"\"";
}
void Write::get_int(void*p,std::string &str)
{
  // char buf[20]={0};
  // myitoa( *( (int*)p ), buf);
  // str+=buf;

   std::ostringstream osout;
   osout<< ( *((int*)p ));
   str+=osout.str();

}
void Write::get_double(void*p,std::string &str)
{
   
	std::ostringstream osout;
   osout<< ( *((double*)p ));
   str+=osout.str();

/*

   char buf[20]={0};
   double dou=*( (double*)p );
   int di=dou;
   myitoa( di, buf);
   str+=buf;//先获取整型部分

   double xiao;//正数的小数部分；
   char h[20]={0};
   h[0]='.';
   int hi=1;
   int zheng;
   if (dou>0){
   	  xiao=dou-di;
   }
   else{
	  xiao=-(dou-di);
   }

   while(1){
   	xiao*=10;
   	zheng=xiao;
   	xiao-=zheng;
   	if (xiao==0 || hi==19)
   		break;
   	h[hi++]=zheng+'0';//把小数部分放入h字符串中
   }
   str+=h;
*/
}
void Write::get_object(void*p,std::string &str)
{
	ValueObj *xp=(ValueObj*)p;
	run1(*xp,str);
}
void Write::get_array(void*p,std::string &str)
{
	ValueArr *xp=(ValueArr*)p;
	run1(*xp,str);
}
void Write::get_bool(void*p,std::string &str)
{
	bool *xp=(bool*)p;
	if (*xp==true)
	{
		str+="true";
	}
	else if (*xp==false)
	{
		str+="false";
	}
}
void Write::get_null(void*p,std::string &str)
{
	str+="null";
}

/*
void Write::myitoa(int a , char *str)
{
	char st[20]={0};
	int sti=0;
	if (a<0){
		st[sti++]='-';
		a=-a;
	}

	while(1)
	{
      st[sti++]=(a%10)+'0';
      if ((a=a/10) ==0)
      	break;
	}
	int j=0;
	for (int i = sti-1; i >=0 ; --i)//反转过来
	{
		str[j++]=st[i];
	}

}
*/