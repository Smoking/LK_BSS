// CardManage.h: interface for the CCardManage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARDMANAGE_H__F134D222_6BF0_4181_A817_CE723B17EF95__INCLUDED_)
#define AFX_CARDMANAGE_H__F134D222_6BF0_4181_A817_CE723B17EF95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PortManage.h"
#include "ErrorCodeRes.h"
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include "tc_32.h"
#include <string>

using namespace std;
using namespace log4cxx;

#pragma comment(lib, "tc_32.lib")

class CCardManage  
{
public:
	CCardManage();
	virtual ~CCardManage();
	long open(const PortPara &para);
	void close();
	long readData(string &strData); 
	long  writeCard(int windowNo, int basketNo,int addMedflag = 0);
	void clearData();  
	HANDLE icdev;
	static LoggerPtr rootLogger;
	int runMode; 
};

#endif // !defined(AFX_CARDMANAGE_H__F134D222_6BF0_4181_A817_CE723B17EF95__INCLUDED_)
