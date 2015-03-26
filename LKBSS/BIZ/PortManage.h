#if !defined(PortManage_define)
#define PortManage_define

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_	VER > 1000
// PortManage.h : header file
//

#include "Serialport.h"
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

#include <string>

using namespace std;
using namespace log4cxx;

struct PortPara
{
	//初始化参数，串口(并口)信息等
	int port;		//串口号
	unsigned long baudRate;	//波特率
    PortPara& operator=(const PortPara &ptPara)
    {
        this->port = ptPara.port;
        this->baudRate = ptPara.baudRate;
        return *this;
    }
};

class CPortManage
{
public:
	CPortManage();
	~CPortManage();
	long open(const PortPara &para);
	void close();
	long readData(string &strData); //
	long  writeCard(int windowNo, int basketNo);
    
    DWORD readByte(BYTE* pBuf, DWORD dwCount);
    long writeByte(const char *command, int len);   //发送字节 
    void clearData();  //清端口数据
private:
	static LoggerPtr rootLogger;
	CSerialPort port;			//串行端口
    int runMode; 
    PortPara portPara;
};



#endif
