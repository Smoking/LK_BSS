
#include "stdafx.h"
#include "PortManage.h"
#include "Utils.h"
#include <process.h>
#include "ConfigUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;       
#endif


LoggerPtr CPortManage::rootLogger = Logger::getLogger("CPortManage");

CPortManage::CPortManage()
{
	try{
	
	//rootLogger->trace("function CPortManage start..............");
	runMode = configUtil.getInt("runMode");
    memset(&portPara, 0, sizeof(PortPara));
	//rootLogger->trace("function CPortManage end..............");
	}catch (...) {
		/// AfxMessageBox("CPortManage--CPortManage error.");
		rootLogger->fatal("CPortManage--CPortManage error ");
	}
}

CPortManage::~CPortManage()
{
	//rootLogger->trace("function ~CPortManage start..............");
	
	//rootLogger->trace("function ~CPortManage end..............");
}

/************************************************************************/
/* �򿪶˿ڣ�ʧ�ܷ��ش����룬�ɹ�����0
/* para����ӡ�˿ڲ���

   ���ô���ͨ�ų�ʱʱ��

/************************************************************************/
long CPortManage::open(const PortPara &para)
{
	try{
	
	//rootLogger->trace("function open start..............");
    if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
    
	try
	{
		if (port.IsOpen())
		{
			//rootLogger->trace("function open end..............");
			return 0;
		}

		if (para.port <= 0 || para.port > 255)
		{
            rootLogger->error("port error! port = " + Utils::itos(para.port));
			return ER_PORT_NUM;
		}

        portPara = para;

		port.Open(para.port, para.baudRate);
		/*port.Set0WriteTimeout();
		port.Set0ReadTimeout(); */ 
        COMMTIMEOUTS comTimeOut;
        
        // ����ʱ�����ַ�������ʱ��
        comTimeOut.ReadIntervalTimeout = 1000;
        
        // ��ȡÿ�ֽڵĳ�ʱ
        comTimeOut.ReadTotalTimeoutMultiplier = 1000;
        
        // ���������ݵĹ̶���ʱ
        // �ܳ�ʱ = ReadTotalTimeoutMultiplier * �ֽ��� + ReadTotalTimeoutConstant
        comTimeOut.ReadTotalTimeoutConstant = 1000;
        
        // дÿ�ֽڵĳ�ʱ
        comTimeOut.WriteTotalTimeoutMultiplier = 1000;
        
        // д�������ݵĹ̶���ʱ
        comTimeOut.WriteTotalTimeoutConstant = 1000;
        
        // ����ʱ����д���豸����
        port.SetTimeouts(comTimeOut); 
	}catch (CSerialException* pEx)
	{
		string tmp(pEx->GetErrorMessage().GetBuffer(10));
		rootLogger->error("failed to open port" + Utils::itos(para.port) +  ": " + tmp);
		pEx->Delete();
		return ER_OPEN_PORT;
	}	
	//rootLogger->trace("function open end..............");
	return 0;
	}catch (...) {
		rootLogger->fatal("CPortManage--open error ");
		return -1;
	}
}

void CPortManage::close()
{
	try{
	
	//rootLogger->trace("function close start..............");
    if (port.IsOpen())
    {     
       	port.Close();
    }

	//rootLogger->trace("function close end..............");
	}catch (...) {
		/// AfxMessageBox("CPortManage--close error.");
		rootLogger->fatal("CPortManage--close error ");
	}
}


long CPortManage::readData(string &strData)
{
	//rootLogger->trace("function readData start..............");
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
    
    try
    {
        if (!port.IsOpen())
        {
			long lRet = open(portPara);
            if (lRet)
            {
                return lRet;
            }          
        }	
		
		BYTE szCmd[8];
		int nSize = sizeof(szCmd);
		memset(szCmd, 0, nSize);
		szCmd[0] = 0x02 ;        //��ͷ
		szCmd[1] = 0x51;        
		szCmd[2] = 0x00;        
		szCmd[3] = 0x78;        //����
		szCmd[4] = 0x02 ;        
		szCmd[5] = 0x00;        
		szCmd[6] = 0x2B;        
		szCmd[7] = 0x03;     		
		writeByte((char*)szCmd,nSize);
		Sleep(100);
		rootLogger->info("���������к�............");
		int count = 20;
		while (port.BytesWaiting() < 12 && count > 0)			
		{
			count--;
			Sleep(50);
		}	
		strData = "";
		count = port.BytesWaiting();
		if ( count >= 12)
		{
			BYTE *buf  = new BYTE[count];
			memset(buf, 0, count);
			DWORD dwBytesRead = port.Read(buf,count);
			for(int j=0;j<count;j++)
			{
				CString str;
				str.Format("%.2X",buf[j]);
				rootLogger->info(str.GetBuffer(0));
				str.ReleaseBuffer();
			}
			BYTE cHead[7] = {0x02 ,0x51 ,0x00, 0x78 ,0x06, 0x30, 0x03}; 
			BYTE check = cHead[1]^cHead[2]^cHead[3]^cHead[4]^cHead[5] ;
			for (int i = 0; i < dwBytesRead; i++)
			{
				if (cHead[0] == buf[i] && cHead[1] == buf[i+1] && cHead[2] == buf[i+2] && cHead[3] == buf[i+3] 
				    && cHead[4] == buf[i+4] && cHead[5] == buf[i+5])
				{
					for (int j = i+6; j <i+ 12; j++)
					{
						if( j <i+ 10)
						{
							check ^= buf[j];
							CString str;
							str.Format("%.2X",buf[j]);
							strData += str;
						}
						else
						{
							if( j ==i+ 10 && buf[j] != check)
							{
								strData = "";
								delete []buf;
								rootLogger->info("���������кţ�У���ʧ��............");
								return ER_READ_DATA;
							}
							if( j==i+ 11 && buf[j] != cHead[6])
							{
								strData = "";
								delete []buf;
								rootLogger->info("���������кţ�����������............");
								return ER_READ_DATA;
							}
						}
					}
				}
			}
			rootLogger->info("���������кųɹ���strData=" + strData);		
			delete []buf;
		} 
		else
		{
			rootLogger->info("���������к�ʧ��............");
		}
    }
	catch (CSerialException* pEx)
    {
        string tmp(pEx->GetErrorMessage().GetBuffer(10));
        rootLogger->error("failed to read data: " + tmp);
        pEx->Delete();
        return ER_READ_DATA;
    }	
	//rootLogger->trace("function readData end..............");
	return 0;
}

long CPortManage::writeCard(int windowNo, int basketNo)
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	int round = 3;
	while(round>0)
	{
		BYTE szCmd[23];
		BYTE check = 0;
		int nSize = sizeof(szCmd);
		memset(szCmd, 0, nSize);
		szCmd[0] = 0x02 ;        //��ͷ
		szCmd[1] = 0x51;        
		szCmd[2] = 0x00;        
		szCmd[3] = 0x80;        //����
		szCmd[4] = 0x11 ;    
		szCmd[5] = windowNo ;
		szCmd[6] = basketNo / 256 ;
		szCmd[7] = basketNo % 256;
		for(int i =1 ;i< 21;i++)
		{
			check ^= szCmd[i];
		}
		szCmd[21] = check ;
		szCmd[22] = 0x03;     //��β
		
		writeByte((char*)szCmd,nSize);
		rootLogger->info("��ʼд��");
		for(int j=0;j<23;j++)
		{
			CString str;
			str.Format("%.2X",szCmd[j]);
			rootLogger->info(str.GetBuffer(0));
			str.ReleaseBuffer();
		}
		
		Sleep(100);
		BYTE buf[256];
		nSize = sizeof(buf);
		memset(buf, 0, nSize);
		int len = readByte(buf,nSize);
		
		if (len < 8)
		{
			Sleep(100);
			len += readByte(buf+len,nSize-len);
		}
		rootLogger->info("��ȡ���ؽ��");
		for( j=0;j<len;j++)
		{
			CString str;
			str.Format("%.2X",buf[j]);
			rootLogger->info(str.GetBuffer(0));
			str.ReleaseBuffer();
		}
		if (len >=8)
		{
			BYTE cHead[7] = {0x02 ,0x51 ,0x00, 0x80 ,0x02, 0x30, 0x03}; 
			BYTE check = cHead[1] ^ cHead[2] ^cHead[3] ^cHead[4] ^cHead[5] ;
			for( int i= 0; i<len ; i++) 
			{
				if (cHead[0] == buf[i] && cHead[1] == buf[i+1] && cHead[2] == buf[i+2] && cHead[3] == buf[i+3] && cHead[4] == buf[i+4])
				{
					if(cHead[5] == buf[i+5] && check == buf[i+6] && cHead[6] == buf[i+7] )
					{
						rootLogger->info("д���ɹ�");
						return 0;
					}
					else
					{
						rootLogger->info("д������ʧ��");
						round--;
					}
				}
			}			
		}
	}
	return ER_WRITE_CARDATA;

}

DWORD CPortManage::readByte(BYTE* pBuf, DWORD dwCount)
{
	try{
    //rootLogger->trace("function readByte start..............");

    DWORD dwBytesRead = 0;
    try
    {
        if (!port.IsOpen())
        {
            long lRet = open(portPara);
            if (lRet)
            {
                return 0;
            }
            //rootLogger->error("not open the port");
            //return ER_NOT_OPEN_PORT;
        }

        COMSTAT state;
        port.GetStatus(state);
        int count = state.cbInQue;  //���ڽ����ֽ���
        if (count > 0)
        {
            //pBuf  = new BYTE[count];
            rootLogger->debug("count=" + Utils::itos(count));
            dwBytesRead = port.Read(pBuf,dwCount);
        }	
    }catch (CSerialException* pEx)
    {
        string tmp(pEx->GetErrorMessage().GetBuffer(10));
        rootLogger->error("failed to read byte: " + tmp);
        pEx->Delete();
        //return ER_READ_DATA;
        return 0;
    }
    //rootLogger->trace("function readByte end..............");
    return dwBytesRead;
	}catch (...) {
		/// AfxMessageBox("CPortManage--readByte error.");
		rootLogger->fatal("CPortManage--readByte error ");
		return -1;
	}
}

/************************************************************************/
/* ��˿�����
/************************************************************************/
void CPortManage::clearData()
{
	try{
    if (runMode == 1 || runMode == 9)
    {
        return;
    }
    port.ClearReadBuffer();
	}catch (...) {
		/// AfxMessageBox("CPortManage--clearData error.");
		rootLogger->fatal("CPortManage--clearData error ");
	}
}


long CPortManage::writeByte(const char *command, int len)	
{
	try{
	
	//rootLogger->trace("function writeByte start..............");
	try
	{
		if (!port.IsOpen())
		{
            long lRet = open(portPara);
            if (lRet)
            {
                return lRet;
            }
		}
		port.Write(command,len);
	}catch (CSerialException* pEx)
	{
		string tmp(pEx->GetErrorMessage().GetBuffer(10));
		rootLogger->error("failed to send command: " + tmp);
		pEx->Delete();
		return ER_SEND_COMMAND;
	}	
	//rootLogger->trace("function writeByte end..............");
	return 0;
	}catch (...) {
		rootLogger->fatal("CPortManage--writeByte error ");
		return -1;
	}
}

