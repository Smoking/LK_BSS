

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
	static string getAppPath();			//取得当前系统运行路径
	long initConfig(string fileName = "LKBSSConfig.xml");		//初始化，打开配置文件,必须要初始化才能使用
	long loadConfig(bool bAttributeAsKey = false);			//读配置文件至map, bAttributeAsKey = true: 将Attribute值作为map的key值保存
	long saveConfig(map<string, string>::const_iterator begn, map<string, string>::const_iterator end);	//更新键值至map中，同时保存至配置文件
	void scanDevice();							 //扫描到的值设置到私有成员
	//bool getConfig(SysConfigParam *config);					//返回config参数供界面显示用
	//bool setConfig(const SysConfigParam *config); //界面更改的东西接收到私有成员configPara
private:
	//SysConfigParam configPara;
	static LoggerPtr rootLogger;
	TiXmlDocument *configFile;
	string filePath;			//配置文件名
	TiXmlElement *rootElement;	//根元素
	//TiXmlElement *pChild = 0;		//根元素的子节点
	long updateConfig(string key, string value);//更新配置值
};

#endif
