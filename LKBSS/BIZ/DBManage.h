
#if !defined(DBManage_define)
#define DBManage_define

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxmt.h>
#include <winsock2.h>
#include <mysql.h>
#include <vector>
#include <map>
#include <list>
#include <string>
#include "Utils.h"
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include "ErrorCodeRes.h"

using namespace log4cxx;
using namespace std;



class CDBManage
{
public:

	long beginTransaction();        //�������񣬷���0�ɹ�����0ʧ��
	long commitTransaction();       //�ύ������0�ɹ�����0ʧ��
	long rollbackTransaction();     //�ع�������0�ɹ�����0ʧ��
    long endTransaction();          //�������񣨻��Զ��ύ�޸ģ�������0�ɹ�����0ʧ��

	long execTransaction(string sqlText);
	CDBManage();
    CDBManage(bool  flag);

	bool BoxDBflag;

	~CDBManage();
	CMutex *mute;
	void release_Mysql();
	long close();								//�ر�����
	long open(bool  flag);						//
	long open();						//�޲����ӣ�ʹ��Ĭ�ϲ���
	long getServerState();						//����������״̬
	static long backupDB(const string user,const string password,const string db,const string fileName);					//�������ݿ�
	static long recoverDB(const string user,const string password,const string db,const string fileName);					//�ָ����ݿ�
	
	long execSQL(const string &sqlText);				//ִ��SQL���
	long freeRecord();									//�ͷŽ����
	string getFieldValueByIndex(int fieldIndex);		//�����ֶ�����ȡ��ָ���ֶν��,��0��ʼ
	string getFieldValueByName(string fieldName);		//�����ֶ���ȡ��ָ���ֶν�� 
	long nextRow();										//��һ��
	bool isEnd();										//���ݼ��Ƿ�������һ����¼����һ����
	long seekRow(int offset);							//��λ��ָ����¼��0Ϊ��һ����¼
	int getFieldNum();									//�õ��ֶ���
	long getRowNum();									//�õ���¼��
	long getInsertId();									//������INSERT��UPDATE���ΪAUTO_INCREMENT�����ɵ�ֵ
	long updateTable(string strTable, string strWhere, map<string,string> &info);	//�޸ı�����
	/*long fuzzySearch(const string searchField, const string table, 
					 const string whereValue, list<string> &result);	//ģ������*/
private:
	static LoggerPtr rootLogger;
	static bool flag_mysqlIni;
	
	MYSQL *mysql;
	MYSQL_RES *dataSet;					//�����
	MYSQL_ROW row;						//��¼��
	unsigned int fieldNum;				//�ֶ���
	unsigned long rowNum;				//��¼��
	map<string, int> indexMap;			//�ֶ�����ӳ��
	bool bOpen;							//���ݿ�򿪱�־��true���Ѿ���
	
	void resetQuery();					//���ò�ѯ����
	int createIndexMap();				//�����ֶ�����ӳ��
	int getIndex(string key);			//ȡ���ֶ�����
};

#endif
