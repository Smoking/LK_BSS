// MedicineManage.h: interface for the CMedicineManage class.
//author:liuweiping 
//synopsis:ҩƷ����ģ��
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEDICINEMANAGE_H__E4947035_86F7_435A_BA68_D46F35BEF8FC__INCLUDED_)
#define AFX_MEDICINEMANAGE_H__E4947035_86F7_435A_BA68_D46F35BEF8FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>
#include <list>
#include <map>
#include <afxmt.h>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include "DBManage.h"
#include "ErrorCodeRes.h"

using namespace std;
using namespace log4cxx;

#define FAST_TYPE_1 1 //���ٲ���ҩƷ��
#define FAST_TYPE_2 2 //���ٲ���ҩƷ���

//ҩƷλ��״̬�ṹ
typedef struct _MedicinLocationState{
	int location_id;//λ��ID
    int medicine_id; //ҩƷID
    int machine_id; //����ID
	int location_type;  //λ������
	int location_state;//λ��״̬
    int cur_quantity;  //��ǰ����
    int max_capacity; //�������
	int warning_percent;//�������
	int med_warning_falg;//ҩƷ�����־
	int med_publicflag;//����ҩ��־
	string med_name;//ҩƷ����
	string med_unit;//ҩƷ���
	string med_factory;//ҩƷ����
} MedicinLocationState;


//ҩƷ�б�ṹ
typedef struct _MedicinList{
    //ҩƷ���
	int medicineId;
	//HISϵͳҩƷ���
	string medicineHisId;
    //ҩƷ����(�м�,Ƭ����)
	int medicineType;
    //ʣ������
	int leavingQuantity;
	//������
	int medSum;
    //ʣ��ٷֱ�
	int leavingPercent;
	//��Ҫ��ҩ����
	int addmedcount;
	//ҩƷ״̬(ȱҩ��
	string medicineState;
	//ҩƷ��������
    string medicineNameCH;
    //ҩƷ���
	string medUnit;
	//����
	string medFactory;
	//ƴ����
	string medPyCode;

	bool operator < (const _MedicinList& medList) const  
    {  
        if (addmedcount < medList.addmedcount)
        {
            return false;
        }
		if (addmedcount == medList.addmedcount && medSum < medList.medSum)
        {
            return false;
        }
        return true;
    }

} MedicinList;

//�豸ҩƷ������Ϣ
typedef struct _MedinceCount{

	//�豸���
	int deviceID;
	
	//�豸����
	int deviceType;

	//����ַ�������
	int max_capacity;

	//�������
	//int warningPercent;

	//�豸״̬
	int machineState;
	
	//ҩƷ��ǰ����
	int medCount;

	//ҩƷ�������
	int medMaxCount;
    
	//�豸��ַ��
	char locationName[25];

}MedinceCount;

//ҩƷ��Ϣ�ṹ
typedef struct _MedInfo{ 
    //ҩƷ���
	int medicineId;
	//HISϵͳҩƷ���
	string medicineHisId;
	//ҩƷ����(�м�,Ƭ����)
	int medicineType;
	//ҩƷ״̬
	int medicineState;
    //ҩƷ�����
	//int medUnitCapacity;
	//����ַ�����
	int maxCapacity;
	//�������ҩ��
	int maxNumOnce;
    //�������
	int warningPercent;
	//ҩƷ��ǰ����
	int curQuantity;
	//ҩƷ����
	int sumQuantity;
	//ҩƷ��������
	string medicineNameCH;
	//ҩƷӢ������
	string medicineNameEN;
	//��ݱ���
	//string numCode;  //����
	string pyCode;  //ƴ��
	//string stdCode; //��׼
	string custCode; //�û��Զ���
	string medlength;//ҩ�г���
	string medwidth;//ҩ�п��
	string medhigth;//ҩ�и߶�
	//ҩƷ��ע
	string medicineHint;
	//���
	string medicineUnit;
	//��ҩ��λ
    string typeUnit;
    //��������
    string medFactory;
    //��Ч����
	string valiDateTime;
	//ҩƷ����
	string med_batch;
    //ҩƷλ��״̬�б�
	list<MedicinLocationState>locationlist;
}MedInfo;


//�ͻ��������ҩƷ��Ϣ
typedef struct _ClientMedinceInfo{

	//ҩƷ��������
	char medicineNameCH[50];

	//ҩƷ���
	char medicineUnit[20];
	
	//��ǰλ������
	char nPositionAmount[5];

	//��ǰλ���������
	char nPositionCapacity[5];

	//������ǰ����
	char nMachineAmount[5];
	
	//�����������
	char nMachineCapacity[5];
    
	//λ��״̬
	int locationState;
}ClientMedinceInfo;

typedef struct _ClientMedinceLocation{

	//ҩƷ��������
	char medicineNameCH[50];
	
	//ҩƷ���
	char medicineUnit[20];
	
	//��ǰλ���������
	char nPositionCapacity[5];
	
	//������ǰ����
	char nMachineAmount[5];
	
	//�����������
	char nMachineCapacity[5];
	//
	//map<int, int> locationInfo;  //��һ��int��ҩƷ��ַ���ڶ���int����ǰ��λ��ҩƷ��Ŀ
	char locationInfo[1000];
	
	int medtype; 
}ClientMedinceLocation;

typedef struct _ClientLocationSet
{
	int location;		//ҩƷ��ַ
	int maxCapacity;	//�����
	int locationCapacity;//��ǰλ������
}ClientLocationSet;

class CMedicineManage  
{
public:
	static string locationName;
	int updateMedType(int medId,int med_type);
	int getWindowState(map<string,int>* pWin_state);
	int getWindowCount( int & windowCount);
	int getMedWeight(int medID,int &weight);
	int deletelocation(int locationId);//ɾ����ַ��Ϣ
	int Setclientlocationinfo(ClientLocationSet clientlocationse);
	int setWindowfalg(int windowID,int falg);
	int getWindowfalg(list<int>*Windowfalg_list);
	int getMachineState(string machineID,int &state_falg);
	int QueryLocationMedInfo_Client(string medscan, ClientMedinceLocation * clientmedincelocation);
	int QueryMed_allLocationleavingPercent(int medID);
	int clearLocationNum(int location);
	int QueryMedleavingPercent( int  medID,int &falg);
	int QueryMedInfo_Client(int location,ClientMedinceInfo * clientmedinfo);
	int getmedID(int locationID,int &medID);//��ȡҩƷID��
	int clearAllLocation(int medId);//���ҩƷ���е�ַ����
	int getMaxmedID(int &medID);//��ȡҩƷ���ID��
	int deleteMedicine(int med_id);//ɾ��ҩƷ��ϸ��Ϣ
	int addMedicine (MedInfo &medicineinfo);//����ҩƷ��ϸ��Ϣ
	int modifyMedicine(int medID,MedInfo &medicineinfo);//�޸�ҩƷ��ϸ��Ϣ
	int lookMedicine(int medID,MedInfo * pmedicineinfo);//����ҩƷ��ϸ��Ϣ
	int listSearch(list<MedicinList >* pmedicinList,string medicineName="",  int type =1,BOOL medstate=TRUE);//����ҩƷ�б�
	int fastSearch(int type,string input, list<string >* output);//������ٲ���
	int getCurrentMedicineCount(int medID,list<MedinceCount>* medincecount,int flag = 0);//��ȡҩƷ��ǰ����ʣ�������
	int addressSearch (int medID,list<MedicinLocationState>*listAddress);//����ҩƷ�ĵ�ַ�б�
	int pushMedicinLocation(map<int,MedicinLocationState >&medLocationState);//��ҩʱ�޸�ҩƷλ��״̬
	int popMedicinLocation(map<int,MedicinLocationState>&medLocationState);//��ҩʱ�޸�ҩƷλ��״̬
	int modifyMedicinLocation(list<int>location,int falg);//��ƷҩƷλ��״̬
	int getMedicinLocation(map<int,MedicinLocationState> *pmedlocationstate);//���ҩƷλ��״̬
	int initializeMedicinLocation();//��ʼ��ҩƷλ��״̬
	CMedicineManage();
	virtual ~CMedicineManage();
private:
    static map<int,MedicinLocationState>medlocationstate; //ҩƷλ��״̬(�ؼ���:λ��ID)
	static LoggerPtr rootLogger ;
	static bool ini_falg;
	static int m_machinetype;
	static int m_machineID;
	static string max_Muallocation;
	CMutex *mute;
	CDBManage  *db;
};


#endif // !defined(AFX_MEDICINEMANAGE_H__E4947035_86F7_435A_BA68_D46F35BEF8FC__INCLUDED_)
