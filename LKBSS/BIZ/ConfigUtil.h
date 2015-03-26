#if !defined(ConfigUtil_define)
#define ConfigUtil_define

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigUtil.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ConfigUtil

#include <map>
#include <string>
using namespace std;

class ConfigUtil{
private:
	 static map<string,string> m_map;
public:
	static string getValue(string key);
	
	static void setValue(string key, string value);

	static void updateValue(string key, string value);	//更新值
	static int getInt(string key);						//取得整数

	ConfigUtil(){
		//configInitAll();
	};
	
	~ConfigUtil(){};

	//void configInitAll();
};

static ConfigUtil configUtil ;


#endif
