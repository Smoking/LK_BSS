
#include "stdafx.h"
#include "SysConfig.h"
#include "ConfigUtil.h"
#include "Shlwapi.h" //PathRemoveFileSpec

#pragma comment(lib,"shlwapi.lib")   

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;       
#endif

//////////////////////////////////////////////////////////////////////////
//
/*
#define ER_NO_CONFIG_FILE	120001	//没有配置文件
#define ER_CONFIG_FILE		120002	//配置文件错误
#define ER_LOAD_CONFIG_FILE	120003	//加载配置文件失败
#define ER_UPDATE_CONFIG	120004	//更新配置文件失败
*/

//////////////////////////////////////////////////////////////////////////

LoggerPtr CSysConfig::rootLogger = Logger::getLogger("CSysConfig");

/*
修改记录
2010.7.24
去掉读绝对路径
*/
CSysConfig::CSysConfig() 
{
	try{
	
	//rootLogger->trace("function SysConfig start..............");
	string msg = "log4cxx.properties";
	PropertyConfigurator::configure(msg);
	configFile = 0;  //配置文件指针
	//fileName = getAppPath() + "\\LKBSSConfig.xml";
	filePath = /*getAppPath() +*/ ".\\";
	rootElement = 0;	//根元素
	
	//rootLogger->trace("function SysConfig end..............");
	}catch (...) {
		/// AfxMessageBox("CSysConfig--CSysConfig error.");
		rootLogger->fatal("CSysConfig--CSysConfig error ");
	}
}

CSysConfig::~CSysConfig()
{
	try{
	
	if (configFile)
	{
		delete configFile;
		configFile = 0;
	}
	//rootLogger->trace("function ~SysConfig end..............");
	}catch (...) {
		/// AfxMessageBox("CSysConfig--~CSysConfig error.");
		rootLogger->fatal("CSysConfig--~CSysConfig error ");
	}
}

/************************************************************************/
/* 取得当前系统运行路径，返回路径
/* 

/* 2009.10.13
/************************************************************************/
string CSysConfig::getAppPath()			
{
	try{
	
	//rootLogger->trace("function getAppPath start..............");
	TCHAR modulePath[MAX_PATH];
	memset(modulePath, 0, MAX_PATH);
    GetModuleFileName(NULL, modulePath, MAX_PATH); 
	PathRemoveFileSpec(modulePath);
	//rootLogger->trace("function getAppPath end..............");
    return (string)modulePath;
	}catch (...) {
		/// AfxMessageBox("CSysConfig--getAppPath error.");
		rootLogger->fatal("CSysConfig--getAppPath error ");
		return "";
	}
}

/************************************************************************/
/* 初始化，打开配置文件，失败返回错误码，成功返回0
/* 
   2010.1.19 增加fileName
/* 2009.10.13
/************************************************************************/
long CSysConfig::initConfig(string fileName) 
{
	try{
	
	//rootLogger->trace("function initConfig start..............");
	string strFile = filePath + fileName;
	ifstream fin(strFile.c_str());  
	if (!fin)   
	{  
		rootLogger->error("LKBSSConfig file not exist.");
		return ER_NO_CONFIG_FILE;
	}
	configFile = new TiXmlDocument(strFile);
	bool loadOkay = configFile->LoadFile();
	if (!loadOkay)
	{
		delete configFile;
		configFile = 0;
		rootLogger->error("failed to open config file, fileName = " + strFile);
		return ER_CONFIG_FILE;
	}

	if (configFile)
	{
		rootElement = configFile->RootElement();
		/*if (rootElement)
		{
			string key(rootElement->Value());
			rootLogger->debug("rootElement: " + key);
			pChild = rootElement->FirstChildElement();
		}*/
	}

	//rootLogger->trace("function initConfig end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CSysConfig--initConfig error.");
		rootLogger->fatal("CSysConfig--initConfig error ");
		return -1;
	}
}

/************************************************************************/
/* 把配置文件加载至内存，失败返回错误码，成功返回0
/* bAttributeAsKey: 是否将Attribute值作为map的key值保存 true : 是

  2010.1.20 增加bAttributeAsKey
/* 2009.10.12
/************************************************************************/
long CSysConfig::loadConfig(bool bAttributeAsKey)						
{
	try{
	
	//rootLogger->trace("function loadConfig start..............");
	//TiXmlDocument xmlDoc(fileName);
/*	TiXmlElement *rootElement = 0;	//根元素
	
	
	//bool loadOkay = configFile->LoadFile();
	if (configFile)
	{
		rootElement = configFile->RootElement();
*/  if (rootElement)
	{
		string key(rootElement->Value());
		rootLogger->debug("rootElement: " + key);
		TiXmlElement *pChild = 0;		//根元素的子节点
		TiXmlAttribute *codeAttribute;
		pChild = rootElement->FirstChildElement(); 
		while (pChild)
		{
			if(pChild->GetText())
			{	
				if (bAttributeAsKey)
				{
					codeAttribute = pChild->FirstAttribute();
					key = codeAttribute->Value();
				}else
				{
					key = pChild->Value();
				}
				
				string value = pChild->GetText();
				rootLogger->debug(key + " = " + value);
				configUtil.setValue(key, value);
				rootLogger->debug(key + " == " + configUtil.getValue(key));
			}	
			pChild = pChild->NextSiblingElement();
		}
	}else	
	{
		rootLogger->error("failed to load config file!");
		return ER_LOAD_CONFIG_FILE;
	}
	if (configFile)
	{
		delete configFile;
		configFile = 0;
	}
	//rootLogger->trace("function loadConfig end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CSysConfig--loadConfig error.");
		rootLogger->fatal("CSysConfig--loadConfig error ");
		return -1;
	}
}

/************************************************************************/
/* 更新配置值，失败返回错误码，成功返回0
/* 
    2010.7.20 增加写入日志功能
    2010.1.29
/* 2009.10.13
/************************************************************************/
long CSysConfig::updateConfig(string key, string value)
{
	try{
	
	//rootLogger->trace("function updateConfig start..............");
	if (rootElement)
	{
		TiXmlElement *pChild = 0;		//根元素的子节点
		pChild = rootElement->FirstChildElement(key);
		if (pChild)
		{
			rootLogger->debug(pChild->GetText());
			pChild->Clear();
			TiXmlText *NodeContent = new TiXmlText(value); 
			pChild->LinkEndChild(NodeContent);
			//ConfigUtil::updateValue(key,value);  //更新至MAP
            string rsLog = key + "=" + pChild->GetText() + " to " + key + "=" + value;
            Utils::insertSystemlog("1201", "修改系统配置参数", rsLog);
		}
	} else
	{
		rootLogger->error("failed to update configure");
		return ER_UPDATE_CONFIG;
	}
	//rootLogger->trace("function updateConfig end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CSysConfig--updateConfig error.");
		rootLogger->fatal("CSysConfig--updateConfig error ");
		return -1;
	}
}

/************************************************************************/
/* 保存配置信息，(更新键值至map中)，同时保存至配置文件，失败返回错误码，成功返回0
/* map<string, string> : 第一string为要改的字段名，第二string为值

/* 2009.10.13
/************************************************************************/
long CSysConfig::saveConfig(map<string, string>::const_iterator begn, map<string, string>::const_iterator end)							
{	
	try{
	
	//rootLogger->trace("function saveConfig start..............");
	while (begn != end)
	{
		//begn->first;
		long lRet = updateConfig(begn->first,begn->second);
		if (lRet)
		{
			return lRet;
		}
		begn++;
	}
	configFile->SaveFile();
	//rootLogger->trace("function saveConfig end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CSysConfig--saveConfig error.");
		rootLogger->fatal("CSysConfig--saveConfig error ");
		return -1;
	}
}
	
void CSysConfig::scanDevice()							 //扫描到的值设置到私有成员
{
	//rootLogger->trace("function scanDevice start..............");

	//rootLogger->trace("function scanDevice end..............");
}
