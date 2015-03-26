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
	string oper_id;//������
	string user_id;//�û�ID
    string user_name;//�û���
	string event_log;//�����¼�
	string result_log;//�������
	string time_log;//����ʱ��
} SystemlogInfo; 

typedef struct _UserInfo{
	string user_id;//�û�ID
    string user_name;//�û���
	string user_real_name;//�û���ʵ����
	int user_role;//Ȩ����
} UserInfo; 
extern UserInfo userinfo;
//���ݿ����
typedef struct _DatabaseParam			
{
	string host;				//������
	string user;				//�û���
	string password;			//����
	string db;					//���ݿ���
	unsigned int port;			//�˿ڣ�һ��Ϊ0
	string characterSet;
}DatabaseParam;
extern DatabaseParam DBparam;	//���ݿ����Ӳ���

extern DatabaseParam BoxDBparam;	//�м����ݿ����Ӳ���


typedef struct _ADODatabaseParam			
{
	string m_server;				//������
	string m_data;				//ODBC����Դ
	string m_uid;			//�û�
	string m_pwd;					//����
}ADODatabaseParam;
extern ADODatabaseParam ADODBparam;	//ADO���ݿ����Ӳ���

extern unsigned char desKey[8];	//������Կ

namespace Utils{
	string itos(int i);
	string dtos(double d);
	int insertSystemlog(string oper_id,string event_log,string result_log);
	int lookSystemlog(string time_log,list<SystemlogInfo>* inputLog);
	string   ConvertHZToPY(char   *as_HzString) ;
	string ConvertToStandString(string source);
};

#endif
