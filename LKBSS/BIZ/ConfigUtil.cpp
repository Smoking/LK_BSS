#include "stdafx.h"
#include "ConfigUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

map<string,string> ConfigUtil::m_map;

string ConfigUtil::getValue(string key){
	try{
	
	map<string,string>::iterator iter;
	
	iter = m_map.find(key);
	if(iter != m_map.end())
	{
		return iter->second;  
	}else{
		return ""; 
	}
	}catch (...) {
		/// AfxMessageBox("ConfigUtil--getValue error.");
		return "";
	}
}

void ConfigUtil::setValue(string key, string value){
	try{
	
	m_map.insert(pair<string,string>(key,value));
	return;
	}catch (...) {
		/// AfxMessageBox("ConfigUtil--setValue error.");
		return ;
	}
}

/************************************************************************/
/* ���¼�ֵ����������ڣ������ֵ�������ڣ�������ֵ
/*	key����
	value��ֵ
/* 2009.10.14
/************************************************************************/
void ConfigUtil::updateValue(string key, string value) 
{
	try{
	
	map<string,string>::iterator iter;
	
	iter = m_map.find(key);
	if(iter != m_map.end())
	{
		iter->second = value;   
	}else{
		m_map.insert(pair<string,string>(key,value));
	}
	}catch (...) {
		/// AfxMessageBox("ConfigUtil--updateValue error.");
		return ;
	}
}

/************************************************************************/
/*ȡ������ֵ��û���κ��жϣ���������ʱ�᷵��0��ע��ʹ��
//2009.10.14
/************************************************************************/
int ConfigUtil::getInt(string key)
{
	try{
	
	return atoi(getValue(key).c_str());
	}catch (...) {
		/// AfxMessageBox("ConfigUtil--getInt error.");
		return 0;
	}
}
