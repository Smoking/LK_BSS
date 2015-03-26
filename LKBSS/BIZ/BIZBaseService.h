
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
#define HANG_ON 3   // �ֹ�����״̬
#define NOARMAL 0  //����������
#define WAIT_FOR_SEND 3 //�ȴ�����--����״̬
#define FINISH_DISTRIBUTE 1//���ͳɹ�--����״̬
#define FAILED_DISTRIBUTE 2 //����ʧ��--����״̬
#define READY_DISTRIBUTE 0 //�ȴ���ҩ
#define STILL_DISTRIBUTE 1 //��ҩ������
#define DISTRIBUTE_ERROR 3 //��ҩʧ��--����״̬
#define NEED_READDING  7//��Ҫ�Ӳ�ҩ���ڳ���

#define HETER_TYPE 1008
#define TABLET_TYPE 1001
#define BOX_TYPE 1002
#define INJECTION_TYPE 1003
#define ISHELF_TYPE 1004
#define DRUGSHELF_TYPE 1005
#define OTHER_TYPE 1009
#define METER_LOCATION 801
#define OVER_VALIDATE_TIME 210017 //ҩƷ����
#define SPLIT_PRESCRIPTION 180000 //��ִ���
#define IGNORE_PRESCRIPTION 180001 //���󴦷�
#define PRESCRIPTION_DELETE 180013//��������
//********************************************************

class CBIZBaseService {

private:
	static LoggerPtr rootLogger ;

protected:
	int deleteFromDB(int preId,CDBManage* cdb,int splitId);
	int getNewQueues(CDBManage* cdb);  //��ȡ���մ���
	int initQueues(CDBManage* cdb);   //��ʼ������
	WorkQueue getExsitQueue(int fetchState,CDBManage* cdb);   //��ȡ�Ѿ����ڵĶ���
	int getLocationList( int medId,list<CLocation>* locationList);  //��ȡҩƷ��Ӧ��ַ;
	int checkStorage( list<CPrescriptionMedicine>* listMedicine,CDBManage* cdb,int splitId=0 );  //��������

	int updateWorkState(int preId, int splitId,int states,CDBManage* cdb);
	int updatePrescriptionState(int preId,int states,CDBManage* cdb,int splitId=0);
	int updatePrescriptionMedicineListState(int preId,int medicineId,int states,CDBManage* cdb,int splitId=0,int remedy_med_num=0);

	int splitPrescription(int preId, list<CPrescriptionMedicine>* listMedicine,CDBManage* cdb);//��ִ���
	int insertSplitPrescription(int preId,CWorkRec rec,int& splitId,CDBManage* cdb);
	int insertSplitPrescriptionMedicine(int pre_id,int splitId,int medId,int medNum,double medHete,CDBManage* cdb);
	int isDelete(int preId,CDBManage* cdb);
	int updateWindow(CDBManage* cdb);
	int insertPrescriptionInfo(int medid,CDBManage * db);
	int isPubMed(int medId,CDBManage* cdb);
	int pushBackInMedList(list<MedicineInfo>& medList,MedicineInfo medInfo);
	static WorkQueue normalQueue;  //��������
	static WorkQueue abnormalQueue; //�쳣����
	static WorkQueue lastDealQueue; //�������
	static WorkQueue printQueue; //��ӡ����
	
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
	int pauseScanFlag; //��ͣ��־,��������һ��ֹͣ����
	int ControlMode;
	int enableFinishFlag; //�Ƿ����öദ��������־
	int m_addMedflag;
	int runMode;
	
public:
	CBoxMonitor* pBoxMonitor;
	int beginScan();  //��ʼɨ��ѭ��
	int pauseScan();  //��ͣɨ��ѭ��
	int pauseDist();  //������ͣ��־,����һ��ѭ����ʼֹͣ����;
	int resumeScan();  //�ָ�ɨ��ѭ��
	int endScan();    //����ɨ��ѭ��
	int spliceToAbnormalQueue(CDBManage* cdb,int preIdN,int splitId,int preIdA = -1);  //ת�����쳣����
	int spliceToNormalQueue(CDBManage* cdb,int preIdA,int splitId,int preIdN = -1);   //ת������������
	int spliceToNormalQueueMulti(CDBManage* cdb,string sIdQueue);   //����ת������������
	int skipForword(int preId,int splitId);   //ǰ�ƴ���
	int skipBack(int preId,int splitId);      //���˴���
	virtual int distributing(CWorkRec workRec,CDBManage* cdb)=0;   // ��ҩ����
	int sendToQueue(list<CPrescription>* preList , list<CPatient>* patList,CDBManage* cdb);      //���������빤������
	int sendToQueueById(list<int>* preIdList,CDBManage* cdb) ; //���������빤������
	int spliceToAbnormalQueueForRemote(CDBManage* cdb,int preId); //Ϊͨ��ģ��ʹ��,Զ�̹��𴦷�
	
	static unsigned __stdcall ThreadStaticEntryPoint(void * pThis); //���߳̾�̬��� 
	virtual void ThreadEntryPoint()=0;  //���̺߳���
	
	WorkQueue getNormalQueue();   //��ȡ��ҩ����
	WorkQueue getAbnormalQueue();  //��ȡ�������

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
	int getMedicineList(int preId,int splitId,list<CPrescriptionMedicine>& preMedList,CDBManage* db); //��ȡ������Ӧ��ҩ��
	string getBasketId(CDBManage* cdb);
	string getBasketIdFormCard(string cardId,CDBManage* cdb) ;
};
