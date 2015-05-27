#include "read.h"
#include "write.h"
#include <iostream>
#include <string>
using namespace std;
int main()
{

	Read r;
	ValueObj wo;
	string str = "{  \"a\":1,     \"a\":-51111.12,\"b\":\"a' g,f'd[][{\{}{}'f\",\"c\":[\"d,ed\",-20,{\"vv\":true},[-15211,2265],true,null],\"d\":{\"ss\":\"sisi\"}}";
	cout << str;
	if(r.run(str,wo)==false)
	{
		cout << r.json_error;
		return 0;
	}

	cout << "ok"<<endl;

	Write wwo;

	cout << "111111111111111"<<endl;	
	ValueObj copyvo1(wo);//测试ValueObj的复制控制
	ValueObj copyvo2 = wo;//测试ValueObj的复制控制

	getchar();
	string objlea1;
	string objlea2;

	wwo.run(copyvo1, objlea1);
	cout <<"gogogo222"<< endl << objlea1 << endl;

	copyvo1.clean();//////////////////////测试clean();
	wwo.run(copyvo1, objlea1);//测试run(ValueArr& ,std::string &);
	cout <<"gogogo333"<< endl << objlea1 << endl;

	wwo.run(copyvo2, objlea2);//测试ValueObj的复制控制
	cout <<"gogogo4444"<< endl << objlea2 << endl;

	getchar();


	cout << "22222222222222"<<endl;
	string strout;
	wwo.run(wo, strout);
	ValueObj vo;
	r.run(strout, vo);
	cout << strout;

	double a = vo["a"];
	string b = vo["b"];
	cout << a << endl;
	cout << b << endl;

	
	string c0 = vo["c"][0];
	int c1 = vo["c"][1];
	cout << c0 << endl;
	cout << c1 << endl;

	
	//int c2 = vo["c"][2].operator []("a");//vo["c"][2]["a"] 报重载的错误
	//因为“ad”是const char类型，加上operator [](const char *);即解决问题

	cout << "333333333333333"<<endl;
	bool c2 = vo["c"][2]["vv"];
	int c30 = vo["c"][3][0];//【数字】却不会报重载的错误
	int c31 = vo["c"][3][1];
	bool c4 = vo["c"][4];
	void *c5 = vo["c"][5];
	string d = vo["d"].operator []("ss");
	cout << c2 << endl;
	cout << c30 << endl;
	cout << c31 << endl;
	cout << c4 << endl;
	cout << c5 << endl;
	cout << d << endl;


	cout << "33333334444444444"<<endl;
	getchar();
	vo["e"] = "fgfdgfd";//添加节点，测试
	string  e = vo["e"];
	cout << e<< endl;


	vo["c"] = "change c ....";//修改节点，测试
	string cg=vo["c"];
	cout << cg << endl;

	vo["c"][0] = "ccc";
	vo["c"][1] = 123;
	string c0g = vo["c"][0];//修改节点，测试
	int c1g = vo["c"][1];
	cout << c0g << endl;
	cout << c1g << endl;


	cout << "4444444444444444"<<endl;

	vo["d"][0][0]["shendu"][0]["ceshi"] = 1;//修改节点&&深度构造，测试
	vo["d"][0][0]["shendu"][1] = "shendu";
	int shen = vo["d"][0][0]["shendu"][0]["ceshi"];
	string sd = vo["d"][0][0]["shendu"][1];
	cout << shen << endl;
	cout << sd << endl;


	vo["sh"][0][0]["shendu"][0]["ceshi"] = "yes,googogo";//添加节点&&深度构造，测试
	string sde = vo["sh"][0][0]["shendu"][0]["ceshi"];
	cout << sde << endl;


	cout << "55555555555555555"<<endl;
	//复制控制
	copyvo2["sh"][0][0]["shendu"][0]["ceshi"] = "yes,googogo copy2";//添加节点&&深度构造，测试
	vo["sh"][0][0]["shendu"][0]["ceshi"] = "yes,googogo";//用于测试是否会覆盖上一个对象的数据
	string sde1 = copyvo2["sh"][0][0]["shendu"][0]["ceshi"];
	cout << sde1 << endl;

	ValueArr ar;
	string strarr = "[\"d,ed\",-20,{\"vv\":true},[-15211,2265],true,null]";
	cout << str;
	if (r.runarray(strarr, ar) == false)//测试runarray
	{
		cout << r.json_error;
		return 0;
	}
	string arrstr1 = ar[0];
	cout << endl << arrstr1;
	
	copyvo2["sh"][0][0]["shendu"][0]["ceshi"] = ar;//测试ValueArr赋值给TypeValue
	string arrstr1333 = copyvo2["sh"][0][0]["shendu"][0]["ceshi"][0];
	cout << endl << arrstr1333;


	cout << "66666666666666666"<<endl;
	getchar();

	cout<<endl<<wo.havekey("b");//测试havekey
	wo.clean("b");//测试clean(std::string str);
	cout<<endl<<wo.havekey("b");//测试havekey
	
    ar.clean(0);//测试clean(int i);
   	int arrstr13 = ar[0];
	cout << endl << arrstr13;


	cout << "77777777777777777777"<<endl;
	getchar();
	string arrlea1;
	string arrlea2;
	wwo.run(ar,arrlea1);
	cout<<endl<<arrlea1<<endl;
	ar.clean();//////////////////////测试clean();
	wwo.run(ar,arrlea2);//测试run(ValueArr& ,std::string &);
	cout<<endl<<arrlea2<<endl;

	getchar();//析构有问题
	return 0;
}
/*


{
  "a":1,
  "b":"a' g,f'd[][{{}{}'f",
  "c":["d,ed",2,{"vv":1},[1,2],true,null],
  "d":{"ss":1}
}

\"
\\
\/
\b
\f
\n
\r
\t

\u
*/