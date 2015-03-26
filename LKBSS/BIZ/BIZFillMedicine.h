#if !defined(BIZFillMedicine_define)
#define BIZFillMedicine_define

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BIZFillMedicine.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// 
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <list>
#include "Medicine.h"
#include "Location.h"
#include "MedicineManage.h"
#include "dbManage.h"
#include "Singleton.h"
#include <algorithm>
#include "BoxSinglechip.h"

using namespace std;
using namespace log4cxx;

typedef struct  _FillStruct{
	int locationId;
	int machineId;
	int medId;
	string medName;
	int num;
	int lightPosition;

	_FillStruct(){
		this->locationId = 0;
	}

	_FillStruct& operator=(const _FillStruct & fs){
		this->locationId = fs.locationId;
		this->machineId = fs.machineId;
		this->medId = fs.medId;
		this->medName = fs.medName;
		this->num = fs.num;
		this->lightPosition = fs.lightPosition;
		return *this;
	}
}FillStruct;

struct LocDisplay
{
	int locationId;
	int medicineId;
	string medicineName;
	int curCapacity;
	int maxCapacity;
	string medcineUnit;
	
	LocDisplay(){
		this->locationId = 0;
		this->medicineId = 0;
		this->medicineName = "";
		this->curCapacity = 0;
		this->maxCapacity = 0;
		this->medcineUnit="";
		
	}
};

class FindById 
{ 
public: 
    FindById(int Id):desId(Id){} 
    ~FindById(){} 
    bool operator()(int& cwr) const 
    { 
        if(cwr == desId) 
            return true; 
        else 
            return false; 
    } 
private: 
    int desId; 
};//


class CBIZFillMedicine {

private: 
	static LoggerPtr rootLogger ;  //��־��¼��

protected:
	static list<FillStruct> fillList ;   //��ҩ����
	bool autoNext;                //�Զ���ʼ��һ��ҩƷ ��־λ
	static vector<FillStruct> vfs;
	int max_Muallocation;
	string locationName;
	CMedicineManage* cMedicineManage;

	list<CLocation> dispatchMedicine(CMedicine medicine);  //����ҩƷ��ַ
	//static	list<int> runningList;
    
	Singleton<CBoxSinglechip> singletonBoxSinglechip;
	CBoxSinglechip *pBoxSinglechip;

	HWND hwnd;
public:
	int sendLocationToQueue(list<CLocation> locationList,bool clearList=true); //����ַ��ҩ
	int sendMedicineToQueue(list<CMedicine> medicineList); //��ҩƷ��ҩ

	int skipLocation();  //������ǰ��ַ
	int skipMedicine(int& n);  //������ǰҩƷ
	int fillMedicine(int order=0);  //��ҩ
	int fillMedicineRemote(int locationId); //Զ�̼�ҩ
	int initFilling();  //��ʼ��
	int finishFilling(int count,int order=0); //��ɼ�ҩ
	int finishFillingRemote(int count,int locationId,CDBManage* cdb);//Զ����ɼ�ҩ
	LocDisplay getLocationDetail(int locationId,CDBManage* cdb); //��ȡ��ַ��ϸ��Ϣ

	int getValidateLocationId();
	list<FillStruct> getFillList();
	vector<FillStruct> getVfs();
	CBIZFillMedicine();   
	~CBIZFillMedicine();

	void setHwnd(HWND hwnd);
};
#endif
