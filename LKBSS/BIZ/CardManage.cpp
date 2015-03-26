// CardManage.cpp: implementation of the CCardManage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CardManage.h"
#include "ConfigUtil.h"
#include "Utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LoggerPtr CCardManage::rootLogger = Logger::getLogger("CCardManage");

CCardManage::CCardManage()
{
	string msg = configUtil.getValue("logPropFile");
	PropertyConfigurator::configure(msg);
	runMode = configUtil.getInt("runMode");
    icdev = NULL;
}

CCardManage::~CCardManage()
{

}

long CCardManage::open(const PortPara &para)
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	if(icdev==NULL)
	{
		icdev=open_device((unsigned char)(para.port-1) ,para.baudRate);
		if(icdev <= 0)
		{
			icdev = NULL;
			return ER_OPEN_PORT;
		}
		else
		{
			//dev_beep(icdev,2,5,2);
			return 0 ;
		}
	}
	return 0;
}

void CCardManage::close()
{
	if (runMode == 1 || runMode == 9)
	{
		return ;
	}
	if(icdev!=NULL)
	{
		close_device(icdev);
		icdev=NULL;
	}
}

long CCardManage::readData(string &strData)
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	unsigned char snrno[20];
	int errorCode = rf_card(icdev,1,snrno);
	if(0==errorCode)	//icdev:����� 1:Ѱ��ģʽ�� snrno:Ѱ�����صĿ���					
	{
		char snrno1[40];
		hex_asc(snrno,(unsigned char *)snrno1,4);
		CString str;
		str.Format("%s",snrno1);
		strData = str ;
	}
	else
	{
		strData ="";
		rootLogger->info("���������к�ʧ��,�����룺"+Utils::itos(errorCode));
		return ER_READ_DATA;
	}
    return 0;
}
	
long  CCardManage::writeCard(int windowNo, int basketNo,int addMedflag)
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	int round = 3;
	while(round>0)
	{
		unsigned char key[6]={0xff,0xff,0xff,0xff,0xff,0xff};
		if(0!=rf_authentication_key(icdev,0,0x04,key))//icdev:�����0:��֤A���룬0x04:���(4�飬�豸��ת���ɶ�Ӧ������Ҳ����1����)��key:��֤������
		{
			string strcard;
			readData(strcard);
			round--;
			rootLogger->info("д������ʧ��");
			continue ;
		}
		BYTE writedata[16];
		int nSize = sizeof(writedata);
		memset(writedata, 0, nSize);  
		writedata[0] = windowNo ;
		writedata[1] = basketNo / 256 ;
		writedata[2] = basketNo % 256;
		writedata[3] = addMedflag;
		if(0!=rf_write(icdev,0x04,writedata))//icdev:���,0x04:д�Ŀ��(4��)�� writedata1:Ҫд���Ӧ�������
		{
			string strcard;
			readData(strcard);
			round--;
			rootLogger->info("д������ʧ��");
			continue ;
		}
		BYTE readdata[16];
		nSize = sizeof(readdata);
		memset(readdata, 0, nSize); 
		if(0!=rf_read(icdev,0x04,readdata))	//icdev:����� 0x04:д�Ŀ��(4��)�� writedata1:Ҫд���Ӧ�������
		{
			round++ ;
			rootLogger->info("д������ʧ��");
			continue ;
		}
		else if(writedata[0]==readdata[0] && writedata[1]==readdata[1] && writedata[2]==readdata[2])
		{
			return 0 ;
		}
        else
		{
			rootLogger->info("д������ʧ��");
			round++ ;
			continue ;
		}
		
	}
	return ER_WRITE_CARDATA;
}

void CCardManage::clearData()
{
	return ;
}