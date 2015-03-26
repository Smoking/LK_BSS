
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

	long beginTransaction();        //开启事务，返回0成功，非0失败
	long commitTransaction();       //提交，返回0成功，非0失败
	long rollbackTransaction();     //回滚，返回0成功，非0失败
    long endTransaction();          //结束事务（会自动提交修改），返回0成功，非0失败

	long execTransaction(string sqlText);
	CDBManage();
    CDBManage(bool  flag);

	bool BoxDBflag;

	~CDBManage();
	CMutex *mute;
	void release_Mysql();
	long close();								//关闭连接
	long open(bool  flag);						//
	long open();						//无参连接，使用默认参数
	long getServerState();						//服务器连接状态
	static long backupDB(const string user,const string password,const string db,const string fileName);					//备份数据库
	static long recoverDB(const string user,const string password,const string db,const string fileName);					//恢复数据库
	
	long execSQL(const string &sqlText);				//执行SQL语句
	long freeRecord();									//释放结果集
	string getFieldValueByIndex(int fieldIndex);		//根据字段索引取得指定字段结果,从0开始
	string getFieldValueByName(string fieldName);		//根据字段名取得指定字段结果 
	long nextRow();										//下一行
	bool isEnd();										//数据集是否最后（最后一条记录的下一条）
	long seekRow(int offset);							//定位到指定记录，0为第一条记录
	int getFieldNum();									//得到字段数
	long getRowNum();									//得到记录数
	long getInsertId();									//返回由INSERT或UPDATE语句为AUTO_INCREMENT列生成的值
	long updateTable(string strTable, string strWhere, map<string,string> &info);	//修改表数据
	/*long fuzzySearch(const string searchField, const string table, 
					 const string whereValue, list<string> &result);	//模糊查找*/
private:
	static LoggerPtr rootLogger;
	static bool flag_mysqlIni;
	
	MYSQL *mysql;
	MYSQL_RES *dataSet;					//结果集
	MYSQL_ROW row;						//记录集
	unsigned int fieldNum;				//字段数
	unsigned long rowNum;				//记录数
	map<string, int> indexMap;			//字段索引映射
	bool bOpen;							//数据库打开标志，true：已经打开
	
	void resetQuery();					//重置查询数据
	int createIndexMap();				//创建字段索引映射
	int getIndex(string key);			//取得字段索引
};

#endif
