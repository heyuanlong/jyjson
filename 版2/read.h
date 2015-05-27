#ifndef JYREAD_HHH
#define JYREAD_HHH
#include "value.h"

class Read
{  
public:
  Read(){};
  ~Read(){};
    
    std::string json_error;
   // bool check(std::string);//如果有错就输出到error中〿
    void seterror(int ,std::string);

  bool run(std::string str,ValueObj &vob){ 
    return run1(str,&vob).bl;
  }; 
  bool runarray(std::string str,ValueArr &var){ 
    return runarray1(str,&var).bl;
  };


private://辅助性变量类垿
 enum jyjson_Tp{ jyjson_wu, jyjson_shuzhi, jyjson_maohao, jyjson_zhongkuohao, jyjson_huakuohao, jyjson_t, jyjson_f, jyjson_n };
   struct mt
   {
     int num;
     int tpy;//-1无意乿 数倿 引号‿1  [2 {4  true false null 
   };

  typedef struct m
   {
     void* pd;
     bool bl;
   }return_bv;

private:
   void clean_str(std::string &s);
   bool deal_param(std::string &,ValueObj *,struct mt &,int ,int ,int);
   bool deal_param(std::string &,ValueArr *,struct mt &,int ,int);

  return_bv run1(std::string,ValueObj * =NULL);
  return_bv runarray1(std::string,ValueArr * =NULL);

  
  int end_tfn(std::string &,int i,struct mt &);//true false null 本来就是小写皿
  int end_shuzhi1(std::string &,int i,int len);
  int end_shuzhi2(std::string &,int i,int len);//获取数值的最后一个字符的位置
  int next_maohao(std::string &,int i,int len);//获取分段的最后一个字符的位置
  int next_zhongkuohao(std::string &,int i,int len);
  int next_huakuohao(std::string &,int i,int len);

  int deal_next(std::string &,int i,struct mt &,int len);//处理分段
  TypeValue create_shuzhi(std::string &,int c ,int e);//new int或newdouble
};

#endif
