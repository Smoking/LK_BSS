

#if !defined(SYSCONFIG_define)
#define SYSCONFIG_define

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <map>
#include "tinyxml.h"
#include<fstream>

#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include "ErrorCodeRes.h"
#include "Utils.h"

using namespace std;
using namespace log4cxx;
//#pragma comment(lib, "LIBCMT.LIB")


class CSysConfig
{
public:
	CSysConfig(); 
	~CSysConfig();
	static string getAppPath();			//ȡ�õ�ǰϵͳ����·��
	long initConfig(string fileName = "LKBSSConfig.xml");		//��ʼ�����������ļ�,����Ҫ��ʼ������ʹ��
	long loadConfig(bool bAttributeAsKey = false);			//�������ļ���map, bAttributeAsKey = true: ��Attributeֵ��Ϊmap��keyֵ����
	long saveConfig(map<string, string>::const_iterator begn, map<string, string>::const_iterator end);	//���¼�ֵ��map�У�ͬʱ�����������ļ�
	void scanDevice();							 //ɨ�赽��ֵ���õ�˽�г�Ա
	//bool getConfig(SysConfigParam *config);					//����config������������ʾ��
	//bool setConfig(const SysConfigParam *config); //������ĵĶ������յ�˽�г�ԱconfigPara
private:
	//SysConfigParam configPara;
	static LoggerPtr rootLogger;
	TiXmlDocument *configFile;
	string filePath;			//�����ļ���
	TiXmlElement *rootElement;	//��Ԫ��
	//TiXmlElement *pChild = 0;		//��Ԫ�ص��ӽڵ�
	long updateConfig(string key, string value);//��������ֵ
};

#endif
