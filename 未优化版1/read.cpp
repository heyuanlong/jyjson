/*
所有new出来的指针必须有归属
*/


#include "read.h"
#include <cctype>
#include <cstdlib>
#include <string>
#include <sstream>
#include <memory.h>

using namespace std;

Read::return_bv Read::run1(std::string str1, ValueObj *vob)//如果有错就输出到json_error中。
{
	COUTDEBUG
	struct mt  maohao_type;
	ValueObj* vobj;
	if (vob==NULL)
		vobj = new ValueObj;
	else
		vobj = vob;

	std::string str = str1;//方便使用
	clean_str(str);  //先清除空格；

	maohao_type.tpy = -1;//-1无意义
	int len = str.length();//｛。。。。｝的长度

	int colon = 0;//冒号的位置
	int index = 1;//分段的开始字符处
	int endd = 0;//分段的最后一个字符

	if (str[0] != '{')//object的开头必须为｛
	{
		
			seterror(index-1,str);
			return return_bv{vobj,false};

	}

	while (1)
	{

		if (str[index] != '\"')//分段的第一个字符必须是“
		{

			seterror(index,str);
				return return_bv{vobj,false};
		}
		colon = next_maohao(str,index + 1,len) + 1;//冒号的位置
		if(colon==0)
			return return_bv{vobj,false};

		//index是"，index+1开始搜冒号的位置
COUTDEBUG
		if (!deal_next(str, colon + 1, maohao_type, len))//根据冒号后面的第一个字符，来确定maohao_type.typ
		{
			//json_error = "error"; 已经在deal_next中seterror了
				return return_bv{vobj,false};
		}

COUTDEBUG
		if (maohao_type.tpy == jyjson_t
			|| maohao_type.tpy == jyjson_f
			|| maohao_type.tpy == jyjson_n)
			endd = end_tfn(str, colon + 2, maohao_type);

		else if (maohao_type.tpy == jyjson_shuzhi)
			endd = end_shuzhi1(str, colon + 2, len);//冒号后面的第二个字符开始 
	
		else if (maohao_type.tpy == jyjson_maohao)
			endd = next_maohao(str, colon + 2, len);//
           
		else if (maohao_type.tpy == jyjson_zhongkuohao)
			endd = next_zhongkuohao(str, colon + 2, len);

		else if (maohao_type.tpy == jyjson_huakuohao)
			endd = next_huakuohao(str, colon + 2, len);
COUTDEBUG
		if (endd == 0)//等于0，说明分段出有问题
		{
			
			//seterror(colon+2,str);
			return return_bv{vobj,false};
		}
		else
		{
			//根据maohao_type，index,colon，endd来处理这个段
			if (false == deal_param(str, vobj, maohao_type, index, colon, endd))
			{
				COUTDEBUG
				//json_error = "error";已经在deal_param中seterror了
					return return_bv{vobj,false};
			}

		}

		if (str[endd + 1] == '}')//无分段了
		{
			if ((endd + 1 + 1) != len)//无分段了，但是下面还有字符，出错
			{
				seterror(endd,str);
					return return_bv{vobj,false};
			}

			break;//无分段了，分析成功
		}
		else if (str[endd + 1] == ',')//还有其他分段，继续处理
		{
			index = endd + 2;//下一个分句的第一个字符头开始
			continue;
		}
		else//分段后面是除了 ，和｝外，则出错
		{
			seterror(endd,str);
				return return_bv{vobj,false};
		}
	}
	COUTDEBUG
	cout<<endd;
	return return_bv{vobj,true};
}
bool Read::deal_param(std::string &str, ValueObj *vobj, struct mt &mao, int i, int c, int e)
{
COUTDEBUG
	std::string s;
	TypeValue tv(jyjson_nothing,NULL);
	if (str[i] != '\"'
		|| str[c - 1] != '\"')//要求key是”“包围着
	{
		seterror(i,str);
		return false;
	}
	else
	{
		s = str.substr(i + 1, c - i - 2);///获取key字符串
	}
COUTDEBUG
	switch (mao.tpy)//根据值的类型处理
	{
	case jyjson_shuzhi:
	{
						  tv = create_shuzhi(str, c, e);
						  if (tv.type == -1)
							  return false;
	}
	COUTDEBUG	break;
	case jyjson_maohao://字符串
	{
						   tv.type = jyjson_string;
						   tv.vpoint = new std::string(str.substr(c + 2, e - c - 2));
	}
	COUTDEBUG	break;
	case jyjson_zhongkuohao://数组
	{
								tv.type = jyjson_array;
								return_bv bv= runarray1(str.substr(c + 1, e - c));
								tv.vpoint = bv.pd;

								if (bv.bl == false)
									return false;
	}
	COUTDEBUG	break;
	case jyjson_huakuohao://object对象
	{
							  tv.type = jyjson_object;
							  return_bv bv = run1(str.substr(c + 1, e - c));
							  tv.vpoint = bv.pd;

							  if (bv.bl == false)
								  return false;
	}
	COUTDEBUG	break;
	case jyjson_t://true
	{
					  tv.type = jyjson_bool;
					  tv.vpoint = new bool(true);
	}
	COUTDEBUG	break;
	case jyjson_f://false
	{
					  tv.type = jyjson_bool;
					  tv.vpoint = new bool(false);
	}
	COUTDEBUG	break;
	case jyjson_n://null
	{
					  tv.type = jyjson_null;
					  tv.vpoint = NULL;
	}
	COUTDEBUG	break;
	}

	vobj->insert(s, tv.type, tv.vpoint);
	COUTDEBUG
	return true;
}
TypeValue Read::create_shuzhi(std::string &str, int c, int e)
{
	COUTDEBUG
	TypeValue tp(jyjson_nothing, NULL);
	int d = 0;//点的数目
	int j = 0;
	int in;
	double dou;
	int size=e - c + 1;
	char *p = new char[size];
	memset(p, '\0', size);
COUTDEBUG

	p[j++]=str[c+1];
	for (int i = c + 2; i <= e; ++i)
	{
		if (!isdigit(str[i]) && str[i] != '.')//不等于数字或. 错
		{
			tp.type = -1;
			seterror(i,str);
			delete []p;
			return tp;
		}
		if (str[i] == '.')//计算有几个点
		{
			++d;
		}
		p[j++] = str[i];
	}
COUTDEBUG
	if (d == 0)
	{
		if (str[c+1] == '-' && str[c + 2] == '0')////整数-02
		{
			tp.type = -1;
			seterror(c,str);
			delete []p;
			return tp;
		}
		//in = atoi(p);
		istringstream iint(p);
		iint>>in;

		int *vp = new int(in);
		tp.type = jyjson_numint;
		tp.vpoint = vp;
		delete []p;
		return tp;
	}
	else if (d == 1)//有一个点
	{
		if (str[c + 1] == '0' && str[c + 2] != '.')//01.55错  022错,
		{
			tp.type = -1;
			seterror(c,str);
			delete []p;
			return tp;
		}
		if (str[e] == '.')//5656.错
		{
			tp.type = -1;
			seterror(e,str);
			delete []p;
			return tp;
		}
		if (  str[c + 1] == '-' && str[c + 2] == '.'
		   || str[c + 1] == '-' && str[c + 2] == '0')//-.55,-01.55,错
		{
			tp.type = -1;
			seterror(c,str);
			delete []p;
			return tp;
		}

		//dou = atof(p);
		istringstream idou(p);
		idou>>dou;

		double *vp = new double(dou);
		tp.type = jyjson_numdouble;
		tp.vpoint = vp;
		delete []p;
		return tp;
	}
	else//多个点
	{
		tp.type = -1;
		seterror(c+2,str);
		delete []p;
		return tp;
	}
COUTDEBUG


}
void Read::clean_str(std::string &s)
{
	int nopre = 0;//1表示有剩余的\的

	int n = 2;//双数时候，可以清除这个空格；
	//int count = s.length();

	for (int i = 0; i < s.length(); ++i)
	{

		if (s[i] == '\\'  && nopre == 0)//有剩余的\的
			nopre = 1;

		else if (s[i] == '\\' && i> 0 && s[i - 1] == '\\' && nopre == 1)
			nopre = 0;//被这个\匹配了，所以有没了

		else if (s[i] == '\"' && i> 0 && s[i - 1] == '\\' && nopre == 1)
			nopre = 0;//被这个\匹配了，所以有没了

		else if (s[i] == '\"'  && nopre == 0)//引号数目加一
			++n;

		else if ((s[i] == ' ' || s[i] == '\r'
			|| s[i] == '\n' || s[i] == '\t' || s[i] == '\b') && n % 2 == 0)//删除空格
		{
			s.erase(i, 1);
			i--;
		  //if (i < 0)i = 0;
		}
	}
}
int Read::end_tfn(std::string &str, int i, struct mt &maohao_type)//true false null 本来就是小写的
{
	switch (maohao_type.tpy)
	{
	case jyjson_t:
	{
					 if (str[i] == 'r'
						 && str[i + 1] == 'u'
						 && str[i + 2] == 'e')
					 {
						 return i + 2;
					 }
	}
	case jyjson_f:
	{
					 if (str[i] == 'a'
						 && str[i + 1] == 'l'
						 && str[i + 2] == 's'
						 && str[i + 3] == 'e')
					 {
						 return i + 3;
					 }
	}
	case jyjson_n:
	{
					 if (str[i] == 'u'
						 && str[i + 1] == 'l'
						 && str[i + 2] == 'l')
					 {
						 return i + 2;
					 }
	}
	}

	seterror(i,str);
	return 0;
}
int Read::end_shuzhi1(std::string &str, int i, int len)
{
	while (true)
	{
COUTDEBUG
		if (i == len)//{"a":1  错
		{
			seterror(i,str);
			return 0;//挂掉了
		}
		if (str[i] == ',' || str[i] == '}')
		{
			return i - 1;
		}
		++i;
	}
}
int Read::end_shuzhi2(std::string &str, int i, int len)
{
	while (true)
	{
COUTDEBUG
		if (i == len)//{"a":1  错
		{
			seterror(i,str);
			return 0;//挂掉了
		}
		if (str[i] == ',' || str[i] == ']')
		{
			return i - 1;
		}
		++i;
	}
}
int Read::next_maohao(std::string &str, int i, int len)
{
	int nopre = 0;//1表示有剩余的\的
	while (true)
	{

		if (i == len){
			seterror(i-3,str);
			return 0;//挂掉了
		}

		if (str[i] == '\\'  && nopre == 0)//有剩余的\的
			nopre = 1;

		else if (str[i] == '\\' && str[i - 1] == '\\' && nopre == 1)
			nopre = 0;//被这个\匹配了，所以有没了

		else if (str[i] == '\"' && str[i - 1] == '\\' && nopre == 1)
			nopre = 0;//被这个\匹配了，所以有没了

		else if (str[i] == '\"'  && nopre == 0)//引号数目加一
			return i;

		++i;
	}

}
int Read::next_zhongkuohao(std::string &str, int i, int len)
{
	int n = 1;
	while (true)
	{

		if (i == len)
		{
			seterror(i,str);
			return 0;//挂掉了
		}

		if (str[i] == '[')
			++n;
		else if (str[i] == ']')
		{
			--n;
			if (n == 0)
				return i;//找到了
		}
		++i;
	}
}
int Read::next_huakuohao(std::string &str, int i, int len)
{
	int n = 1;
	while (true)
	{

		if (i == len)
		{
			seterror(i,str);
			return 0;//挂掉了
		}

		if (str[i] == '{')
			++n;
		else if (str[i] == '}')
		{
			--n;
			if (n == 0)
				return i;//找到了
		}

		++i;
	}

}
int Read::deal_next(std::string &str, int i, struct mt &maohao_type, int len)
{
	if (i == len)
		{
			seterror(i-3,str);
			return 0;//挂掉了
		}



	if (isdigit(str[i]) || str[i]=='-')
	{
		maohao_type.tpy = jyjson_shuzhi;
		maohao_type.num = 1;
		return 1;
	}
	else if (str[i] == '\"')
	{
		maohao_type.tpy = jyjson_maohao;
		maohao_type.num = 1;
		return 1;
	}
	else if (str[i] == '[')
	{
		maohao_type.tpy = jyjson_zhongkuohao;
		maohao_type.num = 1;
		return 1;
	}
	else if (str[i] == '{')
	{
		maohao_type.tpy = jyjson_huakuohao;
		maohao_type.num = 1;
		return 1;
	}
	else if (str[i] == 't')
	{
		maohao_type.tpy = jyjson_t;
		maohao_type.num = 1;
		return 1;
	}
	else if (str[i] == 'f')
	{
		maohao_type.tpy = jyjson_f;
		maohao_type.num = 1;
		return 1;
	}
	else if (str[i] == 'n')
	{
		maohao_type.tpy = jyjson_n;
		maohao_type.num = 1;
		return 1;
	}
	else
	{
		seterror(i, str);
		return 0;//挂掉了
	}
}




















Read::return_bv Read::runarray1(std::string str, ValueArr *var)
{
	struct mt arr_type;
	int index = 1;
	ValueArr *varr;
	if (var==NULL)
		varr = new ValueArr;
	else
		varr = var;
	int endd;
	int len = str.length();//｛。。。。｝的长度



	if (str[0] != '[')//array的开头必须为[]
	{
		seterror(index-1,str);
			return return_bv{varr,false};
	}
	while (1)
	{
		if (!deal_next(str, index, arr_type, len))//根据分居的第一个字符，来确定arrtype.typ
		{
			//json_error = "error"; 已经在deal_next中seterror了
			return return_bv{varr,false};
		}


		if (arr_type.tpy == jyjson_t
			|| arr_type.tpy == jyjson_f
			|| arr_type.tpy == jyjson_n)
			endd = end_tfn(str, index + 1, arr_type);

		else if (arr_type.tpy == jyjson_shuzhi)
			endd = end_shuzhi2(str, index + 1, len);//分居的的第二个字符开始 
		else if (arr_type.tpy == jyjson_maohao)
			endd = next_maohao(str, index + 1, len);//            
		else if (arr_type.tpy == jyjson_zhongkuohao)
			endd = next_zhongkuohao(str, index + 1, len);
		else if (arr_type.tpy == jyjson_huakuohao)
			endd = next_huakuohao(str, index + 1, len);

		if (endd == 0)//等于0，说明分段出有问题
		{
			seterror(index+1,str);
			return return_bv{varr,false};
		}
		else
		{


			////--------------

			//根据arr_type，index,endd来处理这个段
			if (false == deal_param(str, varr, arr_type, index, endd))//传入的是分句的头和尾
			{
				//json_error = "error";已经在deal_param里面seterror了
				return return_bv{varr,false};
			}

		}



		if (str[endd + 1] == ']')//无分段了
		{
			if (endd + 1 + 1 != len)//无分段了，但是下面还有字符，出错
			{
				seterror(endd,str);
				return return_bv{varr,false};
			}

			break;//无分段了，分析成功
		}
		else if (str[endd + 1] == ',')//还有其他分段，继续处理
		{
			index = endd + 2;//下一个分句的第一个字符头开始
			continue;
		}
		else//分段后面是除了 ，和｝外，则出错
		{
			seterror(endd,str);
			return return_bv{varr,false};
		}
	}
	return return_bv{varr,true};
}
bool Read::deal_param(std::string &str, ValueArr *varr, struct mt &mao, int i, int e)//i和e是这个分句的头和尾
{

	TypeValue tv(jyjson_nothing, NULL);


	switch (mao.tpy)//根据值的类型处理
	{
	case jyjson_shuzhi:
	{
						  tv = create_shuzhi(str, i - 1, e);//i-1的原因是，for循环是从i有用的字符开始的
						  if (tv.type == -1)
							  return false;
	}
		break;
	case jyjson_maohao://字符串
	{
						   tv.type = jyjson_string;
						   tv.vpoint = new std::string(str.substr(i + 1, e - i - 1));
	}
		break;
	case jyjson_zhongkuohao://数组
	{
								tv.type = jyjson_array;
								return_bv bv = runarray1(str.substr(i, e - i + 1));
								tv.vpoint = bv.pd;

								if (bv.bl == false)
									return false;
	}
		break;
	case jyjson_huakuohao://object对象
	{
							  tv.type = jyjson_object;
							  return_bv bv  = run1(str.substr(i, e - i + 1));

							  tv.vpoint = bv.pd;
								if (bv.bl == false)
									return false;
	}
		break;
	case jyjson_t://true
	{
					  tv.type = jyjson_bool;
					  tv.vpoint = new bool(true);
	}
		break;
	case jyjson_f://false
	{
					  tv.type = jyjson_bool;
					  tv.vpoint = new bool(false);
	}
		break;
	case jyjson_n://null
	{
					  tv.type = jyjson_null;
					  tv.vpoint = NULL;
	}
		break;
	}
	varr->insert(tv.type, tv.vpoint);
	return true;
}
void Read::seterror(int j,std::string str)
{    
	ostringstream outerror;
	int len=str.length();
	outerror<<"Error near -";
	outerror<<j;
	outerror<<" character, that is, near string ";
	if (j>0)--j;
	if (j>0)--j;

	for (int i = j; i < j+5; ++i)
	{
		if (len==i)
		    break;
		outerror<<str[i];
	}
	json_error=outerror.str();
}

/*
int a[128];
a['+']=1;///预备优先级
a['-']=1;
a['*']=2;
a['/']=2;
a['%']=2;
a['(']=0;
a[')']=0;
a[',']=-1;
*/
















