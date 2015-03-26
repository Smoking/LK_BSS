
#pragma once
// _MSC_VER > 1000
// BIZBaseService.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// BIZBaseService Entities

#include "workQueue.h"
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <string>
#include "PrescriptionMedicine.h"
#include "Location.h"
#include "Prescription.h"
#include "DBManage.h"
#include "BoxMonitor.h"
#include "ErrorCodeRes.h"
#include <process.h>
#include <windows.h>
#include "Patient.h"
#include "Statistic.h"
#include "PrescriptionManage.h"
#include <vector>
#include "MedicineManage.h"
#include "Singleton.h"
#include "FxPLC.h"
#include "BoxSinglechip.h"
//#include "PortManage.h"
#include "CardManage.h"


using namespace log4cxx;

class FindByPreId 
{ 
public: 
    FindByPreId(int preId):desId(preId){} 
    ~FindByPreId(){} 
    bool operator()(CWorkRec& cwr) const 
    { 
        if(cwr.getPreId()==desId) 
            return true; 
        else 
            return false; 
    } 
private: 
    int desId; 
};//

class FindBySplitId 
{ 
public: 
    FindBySplitId(int splitId):desId(splitId){} 
    ~FindBySplitId(){} 
    bool operator()(CWorkRec& cwr) const 
    { 
        if(cwr.getSplitId()==desId) 
            return true; 
        else 
            return false; 
    } 
private: 
    int desId; 
};//

struct LocDetail
{
	int locationId;
	int medicineId;
	string medicineName;
	int curCapacity;
	int maxCapacity;
	string locationState;
	string medicineType;
	string medicineUnit;

	LocDetail(){
		this->locationId = 0;
		this->medicineId = 0;
		this->medicineName = "";
		this->curCapacity = 0;
		this->maxCapacity = 0;
		this->locationState = "0";
		this->medicineType = "0";
		this->medicineUnit = "";
	}
};
//********************************************************
#define ALREADY_FETCH 1
#define NONE_FETCH 0
#define ALL_FETCH_STATE -1
#define HANG_ON 3   // 手工挂起状态
#define NOARMAL 0  //正常待发送
#define WAIT_FOR_SEND 3 //等待发送--处方状态
#define FINISH_DISTRIBUTE 1//发送成功--处方状态
#define FAILED_DISTRIBUTE 2 //发送失败--处方状态
#define READY_DISTRIBUTE 0 //等待发药
#define STILL_DISTRIBUTE 1 //发药动作中
#define DISTRIBUTE_ERROR 3 //发药失败--队列状态
#define NEED_READDING  7//需要从补药窗口出来

#define HETER_TYPE 1008
#define TABLET_TYPE 1001
#define BOX_TYPE 1002
#define INJECTION_TYPE 1003
#define ISHELF_TYPE 1004
#define DRUGSHELF_TYPE 1005
#define OTHER_TYPE 1009
#define METER_LOCATION 801
#define OVER_VALIDATE_TIME 210017 //药品过期
#define SPLIT_PRESCRIPTION 180000 //拆分处方
#define IGNORE_PRESCRIPTION 180001 //超大处方
#define PRESCRIPTION_DELETE 180013//废弃处方
//********************************************************

class CBIZBaseService {

private:
	static LoggerPtr rootLogger ;

protected:
	int deleteFromDB(int preId,CDBManage* cdb,int splitId);
	int getNewQueues(CDBManage* cdb);  //获取新收处方
	int initQueues(CDBManage* cdb);   //初始化队列
	WorkQueue getExsitQueue(int fetchState,CDBManage* cdb);   //获取已经存在的队列
	int getLocationList( int medId,list<CLocation>* locationList);  //获取药品对应地址;
	int checkStorage( list<CPrescriptionMedicine>* listMedicine,CDBManage* cdb,int splitId=0 );  //分析处方

	int updateWorkState(int preId, int splitId,int states,CDBManage* cdb);
	int updatePrescriptionState(int preId,int states,CDBManage* cdb,int splitId=0);
	int updatePrescriptionMedicineListState(int preId,int medicineId,int states,CDBManage* cdb,int splitId=0,int remedy_med_num=0);

	int splitPrescription(int preId, list<CPrescriptionMedicine>* listMedicine,CDBManage* cdb);//拆分处方
	int insertSplitPrescription(int preId,CWorkRec rec,int& splitId,CDBManage* cdb);
	int insertSplitPrescriptionMedicine(int pre_id,int splitId,int medId,int medNum,double medHete,CDBManage* cdb);
	int isDelete(int preId,CDBManage* cdb);
	int updateWindow(CDBManage* cdb);
	int insertPrescriptionInfo(int medid,CDBManage * db);
	int isPubMed(int medId,CDBManage* cdb);
	int pushBackInMedList(list<MedicineInfo>& medList,MedicineInfo medInfo);
	static WorkQueue normalQueue;  //正常队列
	static WorkQueue abnormalQueue; //异常队列
	static WorkQueue lastDealQueue; //缓存队列
	static WorkQueue printQueue; //打印队列
	
	int sendMedFlag;
	bool dealFlag;
	HANDLE   hth1;
    static unsigned  uiThread1ID;
	DWORD   dwExitCode;
	int sleepTime;
	string threadName;
	
	CStatistic* cStatistic;
	Singleton<CStatistic> singletonStatistic;
	CPrescriptionManager* cpm;
	CMedicineManage* medManage;
	int m_machineId;

	Singleton<CFxPLC> singletonFxPLC;
	CFxPLC *fxcpu;

    Singleton<CBoxSinglechip> singletonBoxSinglechip;
	CBoxSinglechip *pBoxSinglechip;

	//CPortManage* cPortManage1;
	//CPortManage* cPortManage2;

	CCardManage* cPortManage1;
	CCardManage* cPortManage2;
	int m_machineType;
	int keepRec;
	int splitMax;
	HWND hwnd;
	int pauseScanFlag; //暂停标志,用于在下一轮停止运行
	int ControlMode;
	int enableFinishFlag; //是否启用多处方结束标志
	int m_addMedflag;
	int runMode;
	
public:
	CBoxMonitor* pBoxMonitor;
	int beginScan();  //开始扫描循环
	int pauseScan();  //暂停扫描循环
	int pauseDist();  //设置暂停标志,在下一轮循环开始停止运行;
	int resumeScan();  //恢复扫描循环
	int endScan();    //结束扫描循环
	int spliceToAbnormalQueue(CDBManage* cdb,int preIdN,int splitId,int preIdA = -1);  //转移至异常队列
	int spliceToNormalQueue(CDBManage* cdb,int preIdA,int splitId,int preIdN = -1);   //转移至正常队列
	int spliceToNormalQueueMulti(CDBManage* cdb,string sIdQueue);   //批量转移至正常队列
	int skipForword(int preId,int splitId);   //前移处方
	int skipBack(int preId,int splitId);      //后退处方
	virtual int distributing(CWorkRec workRec,CDBManage* cdb)=0;   // 发药操作
	int sendToQueue(list<CPrescription>* preList , list<CPatient>* patList,CDBManage* cdb);      //将处方加入工作队列
	int sendToQueueById(list<int>* preIdList,CDBManage* cdb) ; //将处方加入工作队列
	int spliceToAbnormalQueueForRemote(CDBManage* cdb,int preId); //为通信模块使用,远程挂起处方
	
	static unsigned __stdcall ThreadStaticEntryPoint(void * pThis); //主线程静态入口 
	virtual void ThreadEntryPoint()=0;  //主线程函数
	
	WorkQueue getNormalQueue();   //获取发药队列
	WorkQueue getAbnormalQueue();  //获取挂起队列

	int getNormalRec(int preId, CWorkRec& rec,bool isSplit = false);  
	int getAbnormalRec(int preId, CWorkRec& rec,bool isSplit = false);

	CBIZBaseService();
	~CBIZBaseService();
	void setHwnd(HWND hwnd);
    int sendMedCount(int locationId ,int count);
	int clearLocation(int locationId);
	int clearMedicine(int medicineId ,int pandian_flag,int medcount);
	int clearCard();
	int getPatient(int prescriptionId, CPatient &patient);
	int getMedicineList(int preId,int splitId,list<CPrescriptionMedicine>& preMedList,CDBManage* db); //获取处方对应的药单
	string getBasketId(CDBManage* cdb);
	string getBasketIdFormCard(string cardId,CDBManage* cdb) ;
};
