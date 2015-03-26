#if !defined(Util_define)
#define Util_define

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DBManage.h"
#include "ConfigUtil.h"
#include<iostream>
#include<string>
#include<list>
#include<sstream>
using namespace std;

typedef struct _SystemlogInfo{
	string oper_id;//操作码
	string user_id;//用户ID
    string user_name;//用户名
	string event_log;//操作事件
	string result_log;//操作结果
	string time_log;//操作时间
} SystemlogInfo; 

typedef struct _UserInfo{
	string user_id;//用户ID
    string user_name;//用户名
	string user_real_name;//用户真实姓名
	int user_role;//权限码
} UserInfo; 
extern UserInfo userinfo;
//数据库参数
typedef struct _DatabaseParam			
{
	string host;				//主机名
	string user;				//用户名
	string password;			//密码
	string db;					//数据库名
	unsigned int port;			//端口，一般为0
	string characterSet;
}DatabaseParam;
extern DatabaseParam DBparam;	//数据库连接参数

extern DatabaseParam BoxDBparam;	//盒剂数据库连接参数


typedef struct _ADODatabaseParam			
{
	string m_server;				//服务名
	string m_data;				//ODBC数据源
	string m_uid;			//用户
	string m_pwd;					//密码
}ADODatabaseParam;
extern ADODatabaseParam ADODBparam;	//ADO数据库连接参数

extern unsigned char desKey[8];	//加密密钥

namespace Utils{
	string itos(int i);
	string dtos(double d);
	int insertSystemlog(string oper_id,string event_log,string result_log);
	int lookSystemlog(string time_log,list<SystemlogInfo>* inputLog);
	string   ConvertHZToPY(char   *as_HzString) ;
	string ConvertToStandString(string source);
};

#endif
