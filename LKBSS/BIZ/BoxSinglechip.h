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
        position = 0;		//位置
        count = 0;			//数量
        deviceNumber = 0;   //设备序号
        errNo = 0;          //错误号
    }
	int medId;
	int position;		//位置
	int count;			//数量
	int deviceNumber;   //设备序号
    int errNo;          //错误号

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
	long MergerMedInfo(MedicineInfoList &medInfo);    //合并相同地址
	long fillMedicineFinishBox(MedicineInfo &medInfo);//结束加药
	long fillMedicineStartBox(MedicineInfo &medInfo);//开始加药
	long distributeMed(MedicineInfoList &medInfo);  //发药
    long isDstrOver(MedicineInfoList &errorAddress);//判断发药是否完成
	long setDistributeMedicineFlag(int flag);//设置发药完成状态
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
        int len;        //接收到数据的长度
        int currPos;    //已经处理到的位置
    };
    HWND hwnd;
    enum{BOXES_OF_BOARD = 32};
private:
	long analyseData(BYTE * pChr,int len,bool flag = false);
	long sendMedInfo(MedicineInfoList &medInfo);    //发送药品信息
	long startDistribute();                         //开始发药, 同时清灯状态
	bool sumCheck(BYTE *pData,int len);          //和校验 , 校验正确返回true 
	int getReturn();                                //获取返回信息
	int InitiativeSendCmd();                       //主动发送请求
	void initDate();
	int checkDstr(BYTE *pChr);                //检查发药情况，返回处理字符个数
    bool checkFinish();
    CSerialPort port;
    static LoggerPtr rootLogger;    
    MedicineInfoList medInfoList;   //发药列表 
    int isOver;         //检查是否结束次数
    DataBuf dataBuf;
    map<int,int>linkError_map;
	map<int,int>locationError_map;
	map<int,int>addLinkError_map;
	map<int,int>sendLinkError_map;
	map<int ,int >addMedError_map;
	map<int ,int >sendMedError_map;
    map<int ,list<int> >addMed_map;
	map<int ,list<int> >finishMed_map;
	bool finishfalg; //发药完成标志
	bool initiativeSendfalg; //主动发送请求标志
	int runMode;	//运行模式 
	int m_lastLocationID;
	CMutex *m_mute;

};

#endif // !defined(AFX_BOXSINGLECHIP_H__02E1EC7C_8ADA_4E7D_9EF8_7D74BA5C1196__INCLUDED_)
