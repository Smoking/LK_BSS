
#include "stdafx.h"
#include "DBManage.h"
#include "ConfigUtil.h"

#pragma comment(lib, "libmysql.lib")

/////////////////////////////////////////////////////////////////////////////
//ERROR CODE 
/*
#define ER_DB_INIT						140001	//���ݿ��ʼ��ʧ��
#define ER_DB_CONNECT					140002	//���ݿ�����ʧ��
#define WM_NO_SQLTEXT					140003	//�����豸״̬�����߳�ʧ��
#define ER_EXECUTE_SQL					140004	//ִ��SQL������
#define WM_DATA_END						140005  //����������һ�е���һ��
#define ER_NO_DATASET					140006	//���ݼ�Ϊ��
#define ER_DB_NOT_OPEN					140007  //���ݿ�û�д�
#define ER_SERVER_STATE					140008	//����������״̬����
#define ER_DB_PARAM						140009	//����������״̬����
#define ER_DB_BACKUP					140010	//����������״̬����
#define ER_DB_RECOVER					140011	//����������״̬����
#define ER_UPDATE_TABLE					140012	//���±�ʧ��
*/
//using namespace Utils;
/////////////////////////////////////////////////////////////////////////////


LoggerPtr CDBManage::rootLogger = Logger::getLogger("CDBManage");
bool CDBManage::flag_mysqlIni=false;
CDBManage::CDBManage()
{
	try{
	//mute= new CMutex(FALSE,"CDBManage",NULL);
	//mute->Lock();
	//rootLogger->trace("function CDBManage start..............");
	string msg = configUtil.getValue("logPropFile");
	PropertyConfigurator::configure(msg);
	resetQuery();
	mysql = NULL;
	bOpen = false;
	BoxDBflag=false;
	open();
	//rootLogger->trace("function CDBManage end..............");
	//mute->Unlock();       
	}catch (...) {
		rootLogger->fatal("CDBManage--CDBManage error ");
		//mute->Unlock(); 
	}
}

CDBManage::CDBManage(bool  flag)
{
	try{  
    //mute= new CMutex(FALSE,"CDBManage",NULL);
	//mute->Lock();
	//rootLogger->trace("function CDBManage(string hostIP) start..............");
	string msg = configUtil.getValue("logPropFile");
	PropertyConfigurator::configure(msg);	
	mysql = NULL;
	bOpen = false;
	BoxDBflag=flag;
	open(flag);
	//rootLogger->trace("function CDBManage(string hostIP) end..............");
    //mute->Unlock();	
	}catch (...) {
		rootLogger->fatal("CDBManage--CDBManage(string hostIP) error ");
        //mute->Unlock();	
	}
}

CDBManage::~CDBManage()
{
	try{	
	//rootLogger->trace("function ~CDBManage start..............");
	close();
	//delete mute;	
	//rootLogger->trace("function ~CDBManage end..............");
	}catch (...) {
		rootLogger->fatal("CDBManage--~CDBManage error ");
	}
}


/************************************************************************/
/* ���ò�ѯ����
/* 
   2009.11.4
/* 2009.8.17
/************************************************************************/
void CDBManage::resetQuery()
{
	//rootLogger->trace("function resetQuery start..............");
	dataSet = 0;					//���ݼ�
	row = NULL;						//��¼��
	fieldNum = 0;				    //�ֶ���
	rowNum = 0;						//��¼��
	//rootLogger->trace("function resetQuery end..............");
}


long CDBManage::open(bool flag)								
{
	try{	

	if(bOpen)
		return 0;
	string error;
    if(!flag_mysqlIni)
	{
		mysql_library_init(0, NULL, NULL);
		flag_mysqlIni =true;
	}
	mysql = mysql_init(NULL);	
	if (NULL == mysql)
	{
		rootLogger->error("failed to initialize mysql!");
		return ER_DB_INIT;
	}
    unsigned int timeout = 20;
    //mysql_options(mysql,MYSQL_OPT_CONNECT_TIMEOUT,&timeout);
	//mysql_options(mysql,MYSQL_OPT_READ_TIMEOUT,&timeout);
	//mysql_options(mysql,MYSQL_OPT_WRITE_TIMEOUT,&timeout);
	try{
		my_bool my_true= TRUE;
		mysql_options(mysql,MYSQL_OPT_RECONNECT,&my_true);
		if(NULL == mysql_real_connect(mysql, BoxDBparam.host.c_str(),
			BoxDBparam.user.c_str(), BoxDBparam.password.c_str(), BoxDBparam.db.c_str(), BoxDBparam.port,NULL,0))
		{
			error = mysql_error(mysql);
			rootLogger->error("failed to connect MySQL server! error: " + error);
			return ER_DB_CONNECT;
		}
		string characterSet = BoxDBparam.characterSet;
		//rootLogger->debug("configUtil.getValue(characterSet): " + characterSet);
		if (characterSet != "")
		{	
			mysql_set_character_set(mysql, characterSet.c_str());	
		}
		string setName = mysql_character_set_name(mysql);
		//rootLogger->info("current character set name:" + setName);
	}
	catch(CException *e){
		rootLogger->error("failed to connect MySQL server!");
		e->Delete();
		return ER_DB_CONNECT;
	}
	resetQuery();
	bOpen = true;
	return 0;
	}catch (...) {
		rootLogger->fatal("CDBManage--open error ");
		return -1;
	}
}			

/************************************************************************/
/* ʹ��Ĭ�ϲ��������ݿ����ӣ�ʧ�ܷ��ش����룬�ɹ�����0
/* 
   2009.12.1 ���������ַ���
   2009.11.12
/* 2009.8.17
/************************************************************************/
long CDBManage::open()								
{
	try{	

	if(bOpen)
		return 0;
	string error;
    if(!flag_mysqlIni)
	{
		mysql_library_init(0, NULL, NULL);
		flag_mysqlIni =true;
	}
	mysql = mysql_init(NULL);
	
	if (NULL == mysql)
	{
		rootLogger->error("failed to initialize mysql!");
		return ER_DB_INIT;
	}
    unsigned int timeout = 20;
    //mysql_options(mysql,MYSQL_OPT_CONNECT_TIMEOUT,&timeout);
	//mysql_options(mysql,MYSQL_OPT_READ_TIMEOUT,&timeout);
	//mysql_options(mysql,MYSQL_OPT_WRITE_TIMEOUT,&timeout);
	
	try{
		my_bool my_true= TRUE;
		mysql_options(mysql,MYSQL_OPT_RECONNECT,&my_true);
		if(NULL == mysql_real_connect(mysql, DBparam.host.c_str(),
			DBparam.user.c_str(), DBparam.password.c_str(), DBparam.db.c_str(), DBparam.port,NULL,0))
		{
			error = mysql_error(mysql);
			rootLogger->error("failed to connect MySQL server! error: " + error);
			return ER_DB_CONNECT;
		}
		string characterSet = DBparam.characterSet;
		//rootLogger->debug("configUtil.getValue(characterSet): " + characterSet);
		if (characterSet != "")
		{	
			mysql_set_character_set(mysql, characterSet.c_str());	
		}
		string setName = mysql_character_set_name(mysql);
		//rootLogger->debug("current character set name:" + setName);
	}
	catch(CException *e){
		rootLogger->error("failed to connect MySQL server!");
		e->Delete();
		return ER_DB_CONNECT;
	}
	resetQuery();
	bOpen = true;
	return 0;
	}catch (...) {
		rootLogger->fatal("CDBManage--open error ");
		return -1;
	}
}	
		
void CDBManage::release_Mysql()
{
	mysql_library_end();
}

/************************************************************************/
/* �ر����ݿ����ӣ��ɹ�����0
/* 
   2010.11.22
   ����mysql = NULL;
/* 2009.8.17
/************************************************************************/
long CDBManage::close()								//�ر�����
{
	try
	{	
		//mute->Lock();
		if (bOpen && mysql != NULL)
		{
			freeRecord();
			mysql_close(mysql);
			mysql = NULL;
		}
		bOpen = false;
		//mute->Unlock();
		return 0;
	}catch (...) 
	{
		rootLogger->fatal("CDBManage--close error ");
		//mute->Unlock();
		return -1;
	}
}

/************************************************************************/
/* ȡ�÷���������״̬��������������0, �����ش�����
/* 

/* 2009.8.20
/************************************************************************/
long CDBManage::getServerState()
{
	try{
	//mute->Lock();
	//rootLogger->trace("function getServerState start..............");
	if (mysql == NULL)
	{
		rootLogger->error("MYSQL not open!");
		//mute->Unlock();
		return ER_DB_NOT_OPEN;
	}
	
	int ret = mysql_ping(mysql);
	if (ret != 0)
	{
		rootLogger->error("failed to connect server, errorCode = " + Utils::itos(ret));
		//mute->Unlock();
		return ER_SERVER_STATE;
	}
	//rootLogger->trace("function getServerState end..............");
	//mute->Unlock();
	return 0;
	}catch (...) {
		rootLogger->fatal("CDBManage--getServerState error ");
		//mute->Unlock();
		return -1;
	}
}

/************************************************************************/
/* �������ݿ⣬�ɹ�����0��ʧ�ܷ��ش�����
/* para�����ݿ������Ҫ�����û��������롢Ҫ���ݵ����ݿ���
/* fileName�������ļ�·�����ļ���
/* 2009.8.21
/************************************************************************/
long CDBManage::backupDB(const string user,const string password,const string db, const string fileName)			//�������ݿ�
{
	try{
	
	//rootLogger->trace("function backupDB start..............");
	//string user = para.user;
	//string password = para.password;
	//string db = para.db;

	if (db == "")
	{
		rootLogger->error("database name is empty!");
		return ER_DB_PARAM;
	}

	string command = "/c mysqldump --user=" + user + " --password=" + password + " " + db + ">" + fileName;
	string exeFile = "cmd.exe";
	HINSTANCE rlt; 
	rlt = ShellExecute(NULL,"open",exeFile.c_str(),command.c_str(),NULL,0); 
	if (rlt <= (HINSTANCE)32 )
	{
		//rootLogger->error("failed to backup database, errorCode = " + Utils::itos(rlt));
		return ER_DB_BACKUP;
	}

	//rootLogger->trace("function backupDB end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CDBManage--backupDB error.");
		rootLogger->fatal("CDBManage--backupDB error ");
		return -1;
	}
}

/************************************************************************/
/* �ָ����ݿ⣬�ɹ�����0��ʧ�ܷ��ش�����
/* para�����ݿ������Ҫ�����û��������롢Ҫ�ָ������ݿ���
/* fileName�������ļ�·�����ļ���
/* 2009.8.21
/************************************************************************/
long CDBManage::recoverDB(const string user,const string password,const string db, const string fileName)		//�ָ����ݿ�
{
	try{
	
	//rootLogger->trace("function recoverDB start..............");

	if (db == "")
	{
		rootLogger->error("database name is empty!");
		return ER_DB_PARAM;
	}

	string command = "/c mysql --user=" + user + " --password=" + password + " " + db + "<" + fileName;
	string exeFile = "cmd.exe";
	HINSTANCE rlt; 
	rlt = ShellExecute(NULL,"open",exeFile.c_str(),command.c_str(),NULL,0); 
	if (rlt <= (HINSTANCE)32 )
	{
		return ER_DB_RECOVER;
	}
	
	//rootLogger->trace("function recoverDB end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CDBManage--recoverDB error.");
		rootLogger->fatal("CDBManage--recoverDB error ");
		return -1;
	}
}

/************************************************************************/
/* ȡ���ֶ��������ҵ�����������û��Ӧ����������-1
/* key��Ҫ���ҵļ�ֵ
/* 2009.8.18
/************************************************************************/
int CDBManage::getIndex(string key)
{
	try{	
	map<string, int>::iterator iter;	
	iter = indexMap.find(key);
	if(iter != indexMap.end())
	{
		return iter->second;
	}else{
		return -1;   //û��Ӧ����������
	}
	}catch (...) {
		rootLogger->fatal("CDBManage--getIndex error ");
		return -1;
	}
}

/************************************************************************/
/* �����ֶ�����ӳ��
/* 
/* 2009.8.18
/************************************************************************/
int CDBManage::createIndexMap()
{
	unsigned int i;
	MYSQL_FIELD *fields;
	string key;
	try{
		fields = mysql_fetch_fields(dataSet); //ȡ���ֶ���Ϣ
		if (!indexMap.empty())
		{
			indexMap.clear();
		}
		for(i = 0; i < fieldNum; i++)
		{	
			key = fields[i].name;
			//indexMap[key]=i;
			indexMap.insert(pair<string,int>(key,i));
		}
		return 0 ;
	}catch (...) {
		string error = mysql_error(mysql); 
		rootLogger->fatal("CDBManage--createIndexMap error :"+error);
		for(i = 0; i < fieldNum; i++)
		{	
			rootLogger->fatal( fields[i].name);
		}
		return ER_NO_DATASET;
	}
}
	
/************************************************************************/
/* ִ��SQL��䣬ʧ�ܷ��ش����룬�ɹ�����0
/* sqlText: SQL���
   
   2010.11.22
   ���ӶԷ���������״̬�ļ�飬����Ѿ��Ͽ������ͷ���������
   2009.11.12
/* 2009.8.17
/************************************************************************/
long CDBManage::execSQL(const string &sqlText)
{
	try{
	//mute->Lock();
    int nRet = 0;  
	if (!bOpen)
	{
		if(BoxDBflag)
		{		
			if (open(BoxDBflag))
			{
				string error = mysql_error(mysql); 
				rootLogger->error("failed to open database! error:"+error);
				//mute->Unlock();
				return ER_DB_NOT_OPEN;
			}
		}
		else
		{
			if (open())
			{
				string error = mysql_error(mysql); 
				rootLogger->error("failed to open database! error:"+error);
				//mute->Unlock();
				return ER_DB_NOT_OPEN;
			}
		}
	}

	if (sqlText == "")
	{
		rootLogger->warn("no SQL text!..............");
		//mute->Unlock();
		return WM_NO_SQLTEXT;
	}
    //freeRecord();
	try{
		rootLogger->debug("sqlText = " + sqlText);
		nRet = mysql_query(mysql, sqlText.c_str());	
		if (nRet != 0)
		{
			string error = mysql_error(mysql); 
			rootLogger->error("failed to execute SQL, errorCode = " + Utils::itos(nRet)+" mysql_error :"+error);
			rootLogger->error("failed sql = " + sqlText);
			//mute->Unlock();
			nRet = mysql_query(mysql, sqlText.c_str());	
			if (nRet != 0)
			    return ER_EXECUTE_SQL;
		}
	}catch(CException *e){
		e->Delete();
		rootLogger->error("failed to execute SQL, errorCode = " + Utils::itos(nRet));
		rootLogger->error("failed sql = " + sqlText);
		//mute->Unlock();
		return ER_EXECUTE_SQL;
	}

	dataSet = mysql_store_result(mysql);  //ȡ�ý����
	if (dataSet == NULL)
	{
		rootLogger->debug("no data set.");
		//mute->Unlock();
		return 0;
	}
    mysql_data_seek(dataSet,0); //��λ����һ��
	row = mysql_fetch_row(dataSet);  //ȡ����
	fieldNum = mysql_num_fields(dataSet);
	rowNum = mysql_num_rows(dataSet);
	nRet = createIndexMap();
	if(nRet != 0)
	{
		rootLogger->error("sqlText = " + sqlText);
		//mute->Unlock();
		return ER_NO_DATASET;
	}
	//mute->Unlock();
	return 0;
	}catch (...) {
		rootLogger->fatal("sqlText = " + sqlText);
		string error = mysql_error(mysql); 
		rootLogger->fatal("CDBManage--execSQL error :"+error);
		//mute->Unlock();
		return -1;
	}
}

/************************************************************************/
/* �ͷŽ�������ɹ�����0
/* 
/* 2009.8.18
/************************************************************************/
long CDBManage::freeRecord()									//�ͷŽ����
{
	try{
	
	//rootLogger->trace("function freeRecord start..............");
	
	if (dataSet != NULL)
	{
		mysql_free_result(dataSet);		//�����
		resetQuery();
	}

	//rootLogger->trace("function freeRecord end..............");
	return 0;
	}catch (...) {
		rootLogger->fatal("CDBManage--freeRecord error ");
		return -1;
	}
}

/************************************************************************/
/* �����ֶ�����ȡ��ָ���ֶν���������ֶν��, ��ָ���������NULL
/* fieldIndex: �ֶ���������0��ʼ
    
   2010.12.24 �Ľ����ֶ�ֵΪ�յ��ж� char *chTmp;
   2010.12.6 return NULL -> return ""
/* 2009.8.17
/************************************************************************/
string CDBManage::getFieldValueByIndex(int fieldIndex)		//�����ֶ�����ȡ��ָ���ֶν�� 
{
	string q_value = "";
	try{
	
	if (row == NULL ||row==0)
	{
		rootLogger->info("no rows"); 
		return "";
	}

	if (fieldIndex >= fieldNum || fieldIndex < 0)
	{
		rootLogger->warn("field index out of range"); 
		return "";
	}

	char *chTmp;
    chTmp = row[fieldIndex];

	if (chTmp == 0)  //����NULL����""
	{
		q_value = "";
	}
	else
	{
		q_value = row[fieldIndex]; 
	}
	return q_value;
	}catch (...) {
		rootLogger->fatal("fieldIndex: "+Utils::itos(fieldIndex)+" FieldValue: " + q_value);
		string error = mysql_error(mysql); 
		rootLogger->fatal("CDBManage--getFieldValueByIndex error :"+error);
		return "";
	}
}

/************************************************************************/
/* �����ֶ���ȡ��ָ���ֶν���������ֶν������ָ���������NULL
/* fieldName: �ֶ���������0��ʼ

  2010.12.24 �Ľ����ֶ�ֵΪ�յ��ж� char *chTmp;
   2010.12.21 return NULL -> return ""
/* 2009.8.18
/************************************************************************/
string CDBManage::getFieldValueByName(string fieldName)	 
{
	string q_value = "";
	try{	
		if (row == NULL||row==0)
		{
			rootLogger->info("no rows."); 
			return "";
		}
		int fieldIndex = getIndex(fieldName);
		if (fieldIndex == -1)//û�д��ֶ� 
		{
			rootLogger->error("the field does not exist, fieldName = " + fieldName);
			return "";
		} 
		
		char *chTmp;
		chTmp = row[fieldIndex];
		if (chTmp == 0)  //����NULL����""
		{
			q_value = "";
		}
		else
		{
			q_value = row[fieldIndex]; 
		}
		return q_value;
	}catch (...) {
		rootLogger->fatal("fieldName: " + fieldName+" q_value :"+q_value);
		string error = mysql_error(mysql); 
		rootLogger->fatal("CDBManage--getFieldValueByName error :"+error);		
		return "";
	}
}


/************************************************************************/
/* ��һ�У���������0�����ݼ��������ط�0��WM_DATA_END��
/* 
/* 2009.8.18
/************************************************************************/
long CDBManage::nextRow()
{
	try{	
	if (dataSet == NULL)
	{
		rootLogger->error("no dataset!");
		return ER_NO_DATASET;
	}
	
	row = mysql_fetch_row(dataSet);
	if (row == NULL ||row==0)
	{
		rootLogger->debug("it is the last row.");
		return WM_DATA_END;
	}
	return 0;
	}catch (...) {
		rootLogger->fatal("CDBManage--nextRow error ");
		return -1;
	}
}

/************************************************************************/
/* ���ݼ��Ƿ�������һ����¼����һ�������� ����true
/* 
/* 2009.8.18
/************************************************************************/
bool CDBManage::isEnd()										
{
	try{
	if (row != NULL && row!=0)
	{
		return false;	
	}
	return true;
	}catch (...) {
		rootLogger->fatal("CDBManage--isEnd error ");
		return true;
	}
}

/************************************************************************/
/* ��λָ��������
/* offset��ָ��λ�ã�0Ϊ��һ����¼
/* 2009.8.18
/************************************************************************/
long CDBManage::seekRow(int offset)							
{
	try{
	if (dataSet == NULL)
	{
		rootLogger->error("no dataset!");
		return ER_NO_DATASET;
	}

	rootLogger->debug("offset = " + Utils::itos(offset));
	mysql_data_seek(dataSet,offset);
	row = mysql_fetch_row(dataSet);
	return 0;
	}catch (...) {
		rootLogger->fatal("CDBManage--seekRow error ");
		return -1;
	}
}

/************************************************************************/
/* �õ��ֶ����������ֶ���
/* 

/* 2009.8.17
/************************************************************************/
int CDBManage::getFieldNum()									
{
	return fieldNum;
}

/************************************************************************/
/* �õ���¼�������ؼ�¼��
/* 
/* 2009.8.18
/************************************************************************/
long CDBManage::getRowNum()									
{
	return rowNum;
}

/************************************************************************/
/* ��������ǰ��INSERT��UPDATE���ΪAUTO_INCREMENT�����ɵ�ֵ, 
/* ���ǰ������δʹ��AUTO_INCREMENT������0, 
/* 
/* 2009.8.19
/************************************************************************/
long CDBManage::getInsertId()
{
	try{
	if (mysql == NULL)
	{
		rootLogger->error("MYSQL not open!");
		return 0;
	}
	return (mysql_insert_id(mysql));
	}catch (...) {
		rootLogger->fatal("CDBManage--getInsertId error ");
		return 0;
	}
}

/************************************************************************/
/* ���£��޸ģ���ʧ�ܷ��ش����룬�ɹ�����0
/* strTable����Ҫ�޸ĵı�
/* strWhere��where���
   info: �޸��ֶ���Ϣ��map�ĵ�һ��ֵΪ�ֶ������ڶ���ֵΪ�ֶ�ֵ

/* 2009.9.7
/************************************************************************/
long CDBManage::updateTable(string strTable, string strWhere, map<string,string> &info)	
{
	try{
	string tmp;
	string sqlText = "update " + strTable + " set ";
	map<string, string>::iterator iter;	
	for(iter = info.begin(); iter != info.end(); iter++)		
	{
		tmp = iter->first + "='" + iter->second + "',";
		sqlText += tmp;
	}
	sqlText.erase(sqlText.end() - 1);  //ɾ����󶺺�
	sqlText = sqlText + " " + strWhere;
	rootLogger->debug("sqlText = " + sqlText);
	long lRet = execSQL(sqlText);
	if (lRet != 0)
	{
		rootLogger->trace("failed to update table,errorCode:" + Utils::itos(lRet));
		return ER_UPDATE_TABLE;
	}
	return 	0;
	}catch (...) {
		rootLogger->fatal("CDBManage--updateTable error ");
		return -1;
	}
}

/************************************************************************/
/* ģ�����ң�ʧ�ܷ��ش����룬�ɹ�����0
searchField: ģ����ѯ�ֶ�
table: ��ѯ�ı�
whereValue: where�����ֶ�ֵ(searchField��ֵ)
result: ��ѯ���

/* 2009.11.25
/************************************************************************/
/*
long CDBManage::fuzzySearch(const string searchField, const string table, 
					 const string whereValue, list<string> &result)
{
	rootLogger->trace("function fuzzySearch start..............");
	string sqlText = "select " + searchField + " from " + table + " where " + searchField + " like '"
					 + whereValue + "%'";
	rootLogger->debug("sqlText = " + sqlText);
	long lRet = execSQL(sqlText);
	if (lRet != 0)
	{
		rootLogger->error("failed to search, code: " + Utils::itos(lRet));
		return lRet;
	}
	while (!isEnd())
	{
		result.push_back(getFieldValueByIndex(0));
		nextRow();
	}
	rootLogger->trace("function fuzzySearch end..............");
	return 0;
}*/

//����
long CDBManage::execTransaction(string sqlText)
{
	try{
    int nRet;
	if (sqlText == "")
	{
		rootLogger->warn("no SQL text!..............");
		return WM_NO_SQLTEXT;
	}

	try{
		rootLogger->debug("sqlText = " + sqlText);
		nRet = mysql_real_query(mysql, sqlText.c_str(),(unsigned long)sqlText.length());	
		if (nRet != 0)
		{
			rootLogger->error("failed to execute SQL, errorCode = " + Utils::itos(nRet));
			rootLogger->error("failed sql = " + sqlText);
			return ER_EXECUTE_SQL;
		}
	}catch(CException *e){
		e->Delete();
		rootLogger->error("failed to execute SQL, errorCode = " + Utils::itos(nRet));
		rootLogger->error("failed sql = " + sqlText);
		return ER_EXECUTE_SQL;
	}
    return 0;
	}catch (...) {
		/// AfxMessageBox("CDBManage--execTransaction error.");
		rootLogger->fatal("CDBManage--execTransaction error ");
		return -1;
	}
}

/************************************************************************/
/* �������񣬷���0�ɹ�����0ʧ��

/* 2011.7.23
/************************************************************************/
long CDBManage::beginTransaction()
{
	try{
    return mysql_autocommit(mysql, 0);
	}catch (...) {
		/// AfxMessageBox("CDBManage--beginTransaction error.");
		rootLogger->fatal("CDBManage--beginTransaction error ");
		return -1;
	}
}

/************************************************************************/
/* �ύ������0�ɹ�����0ʧ��

/* 2011.7.23
/************************************************************************/
long CDBManage::commitTransaction()
{
	return 0;
	try{
    return mysql_commit(mysql);
	}catch (...) {
		/// AfxMessageBox("CDBManage--commitTransaction error.");
		rootLogger->fatal("CDBManage--commitTransaction error ");
		return -1;
	}
}

/************************************************************************/
/* �ع�������0�ɹ�����0ʧ��

/* 2011.7.23
/************************************************************************/
long CDBManage::rollbackTransaction()
{
	try{
    return mysql_rollback(mysql);
	}catch (...) {
		/// AfxMessageBox("CDBManage--rollbackTransaction error.");
		rootLogger->fatal("CDBManage--rollbackTransaction error ");
		return -1;
	}
}

/************************************************************************/
/* �������񣨻��Զ��ύ�޸ģ�������0�ɹ�����0ʧ��

/* 2011.7.23
/************************************************************************/
long CDBManage::endTransaction()
{
	try{
    return mysql_autocommit(mysql, 1);
	}catch (...) {
		/// AfxMessageBox("CDBManage--endTransaction error.");
		rootLogger->fatal("CDBManage--endTransaction error ");
		return -1;
	}
}
