// BoxSinglechip.h: interface for the CBoxSinglechip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOXSINGLECHIP_H__02E1EC7C_8ADA_4E7D_9EF8_7D74BA5C1196__INCLUDED_)
#define AFX_BOXSINGLECHIP_H__02E1EC7C_8ADA_4E7D_9EF8_7D74BA5C1196__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Serialport.h"
#include <afxmt.h>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include "ErrorCodeRes.h"
#include <string>
#include <list>
#include <map>
#include <algorithm>

using namespace std;
using namespace log4cxx;

struct MedicineInfo
{
    MedicineInfo()
    {
		medId = 0;
        position = 0;		//λ��
        count = 0;			//����
        deviceNumber = 0;   //�豸���
        errNo = 0;          //�����
    }
	int medId;
	int position;		//λ��
	int count;			//����
	int deviceNumber;   //�豸���
    int errNo;          //�����

    bool operator < (const MedicineInfo& medInfo) const  
    {  
        if (position < medInfo.position)
        {
            return true;
        }
        
        return false;
    }
};

typedef list<MedicineInfo> MedicineInfoList;

class FindByPos 
{ 
public: 
    FindByPos(int nPos):desId(nPos){} 
    ~FindByPos(){} 
    bool operator()(MedicineInfo& mif) const 
    { 
        if(mif.position==desId) 
            return true; 
        else 
            return false;  
    } 
private: 
    int desId; 
};

class CBoxSinglechip  
{
public:
	CBoxSinglechip();
	virtual ~CBoxSinglechip();
	long getLastLocationID();
	bool getErrorMessage();
	long MergerMedInfo(MedicineInfoList &medInfo);    //�ϲ���ͬ��ַ
	long fillMedicineFinishBox(MedicineInfo &medInfo);//������ҩ
	long fillMedicineStartBox(MedicineInfo &medInfo);//��ʼ��ҩ
	long distributeMed(MedicineInfoList &medInfo);  //��ҩ
    long isDstrOver(MedicineInfoList &errorAddress);//�жϷ�ҩ�Ƿ����
	long setDistributeMedicineFlag(int flag);//���÷�ҩ���״̬
    long open();
    void close();	
public:
    struct DataBuf
    {
        DataBuf()
        {
            init();
        }
        
        void init()
        {
            memset(buf, 0, 1024);        
            len = 0;
            currPos = 0;
        }
        
        BYTE buf[1024];
        int len;        //���յ����ݵĳ���
        int currPos;    //�Ѿ�������λ��
    };
    HWND hwnd;
    enum{BOXES_OF_BOARD = 32};
private:
	long analyseData(BYTE * pChr,int len,bool flag = false);
	long sendMedInfo(MedicineInfoList &medInfo);    //����ҩƷ��Ϣ
	long startDistribute();                         //��ʼ��ҩ, ͬʱ���״̬
	bool sumCheck(BYTE *pData,int len);          //��У�� , У����ȷ����true 
	int getReturn();                                //��ȡ������Ϣ
	int InitiativeSendCmd();                       //������������
	void initDate();
	int checkDstr(BYTE *pChr);                //��鷢ҩ��������ش����ַ�����
    bool checkFinish();
    CSerialPort port;
    static LoggerPtr rootLogger;    
    MedicineInfoList medInfoList;   //��ҩ�б� 
    int isOver;         //����Ƿ��������
    DataBuf dataBuf;
    map<int,int>linkError_map;
	map<int,int>locationError_map;
	map<int,int>addLinkError_map;
	map<int,int>sendLinkError_map;
	map<int ,int >addMedError_map;
	map<int ,int >sendMedError_map;
    map<int ,list<int> >addMed_map;
	map<int ,list<int> >finishMed_map;
	bool finishfalg; //��ҩ��ɱ�־
	bool initiativeSendfalg; //�������������־
	int runMode;	//����ģʽ 
	int m_lastLocationID;
	CMutex *m_mute;

};

#endif // !defined(AFX_BOXSINGLECHIP_H__02E1EC7C_8ADA_4E7D_9EF8_7D74BA5C1196__INCLUDED_)
