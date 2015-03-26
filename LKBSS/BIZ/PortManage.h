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
	//��ʼ������������(����)��Ϣ��
	int port;		//���ں�
	unsigned long baudRate;	//������
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
    long writeByte(const char *command, int len);   //�����ֽ� 
    void clearData();  //��˿�����
private:
	static LoggerPtr rootLogger;
	CSerialPort port;			//���ж˿�
    int runMode; 
    PortPara portPara;
};



#endif
