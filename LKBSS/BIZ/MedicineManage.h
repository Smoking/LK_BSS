// MedicineManage.h: interface for the CMedicineManage class.
//author:liuweiping 
//synopsis:药品管理模块
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

#define FAST_TYPE_1 1 //快速查找药品名
#define FAST_TYPE_2 2 //快速查找药品编号

//药品位置状态结构
typedef struct _MedicinLocationState{
	int location_id;//位置ID
    int medicine_id; //药品ID
    int machine_id; //机器ID
	int location_type;  //位置类型
	int location_state;//位置状态
    int cur_quantity;  //当前数量
    int max_capacity; //最大数量
	int warning_percent;//警告比例
	int med_warning_falg;//药品警告标志
	int med_publicflag;//公用药标志
	string med_name;//药品名称
	string med_unit;//药品规格
	string med_factory;//药品厂家
} MedicinLocationState;


//药品列表结构
typedef struct _MedicinList{
    //药品编号
	int medicineId;
	//HIS系统药品编号
	string medicineHisId;
    //药品类型(盒剂,片剂等)
	int medicineType;
    //剩余数量
	int leavingQuantity;
	//总数量
	int medSum;
    //剩余百分比
	int leavingPercent;
	//需要加药数量
	int addmedcount;
	//药品状态(缺药）
	string medicineState;
	//药品中文名称
    string medicineNameCH;
    //药品规格
	string medUnit;
	//厂家
	string medFactory;
	//拼音码
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

//设备药品数量信息
typedef struct _MedinceCount{

	//设备编号
	int deviceID;
	
	//设备类型
	int deviceType;

	//单地址最大容量
	int max_capacity;

	//警告比例
	//int warningPercent;

	//设备状态
	int machineState;
	
	//药品当前数量
	int medCount;

	//药品最大数量
	int medMaxCount;
    
	//设备地址表
	char locationName[25];

}MedinceCount;

//药品信息结构
typedef struct _MedInfo{ 
    //药品编号
	int medicineId;
	//HIS系统药品编号
	string medicineHisId;
	//药品类型(盒剂,片剂等)
	int medicineType;
	//药品状态
	int medicineState;
    //药品规格量
	//int medUnitCapacity;
	//单地址最大数
	int maxCapacity;
	//单次最大发药量
	int maxNumOnce;
    //警告比例
	int warningPercent;
	//药品当前余量
	int curQuantity;
	//药品总量
	int sumQuantity;
	//药品中文名称
	string medicineNameCH;
	//药品英文名称
	string medicineNameEN;
	//快捷编码
	//string numCode;  //数字
	string pyCode;  //拼音
	//string stdCode; //标准
	string custCode; //用户自定义
	string medlength;//药盒长度
	string medwidth;//药盒宽度
	string medhigth;//药盒高度
	//药品备注
	string medicineHint;
	//规格
	string medicineUnit;
	//摆药单位
    string typeUnit;
    //生产厂家
    string medFactory;
    //有效日期
	string valiDateTime;
	//药品批号
	string med_batch;
    //药品位置状态列表
	list<MedicinLocationState>locationlist;
}MedInfo;


//客户端需求的药品信息
typedef struct _ClientMedinceInfo{

	//药品中文名称
	char medicineNameCH[50];

	//药品规格
	char medicineUnit[20];
	
	//当前位置数量
	char nPositionAmount[5];

	//当前位置最大容量
	char nPositionCapacity[5];

	//本机当前数量
	char nMachineAmount[5];
	
	//本机最大容量
	char nMachineCapacity[5];
    
	//位置状态
	int locationState;
}ClientMedinceInfo;

typedef struct _ClientMedinceLocation{

	//药品中文名称
	char medicineNameCH[50];
	
	//药品规格
	char medicineUnit[20];
	
	//当前位置最大容量
	char nPositionCapacity[5];
	
	//本机当前数量
	char nMachineAmount[5];
	
	//本机最大容量
	char nMachineCapacity[5];
	//
	//map<int, int> locationInfo;  //第一个int：药品地址，第二个int：当前槽位该药品数目
	char locationInfo[1000];
	
	int medtype; 
}ClientMedinceLocation;

typedef struct _ClientLocationSet
{
	int location;		//药品地址
	int maxCapacity;	//最大量
	int locationCapacity;//当前位置数量
}ClientLocationSet;

class CMedicineManage  
{
public:
	static string locationName;
	int updateMedType(int medId,int med_type);
	int getWindowState(map<string,int>* pWin_state);
	int getWindowCount( int & windowCount);
	int getMedWeight(int medID,int &weight);
	int deletelocation(int locationId);//删除地址信息
	int Setclientlocationinfo(ClientLocationSet clientlocationse);
	int setWindowfalg(int windowID,int falg);
	int getWindowfalg(list<int>*Windowfalg_list);
	int getMachineState(string machineID,int &state_falg);
	int QueryLocationMedInfo_Client(string medscan, ClientMedinceLocation * clientmedincelocation);
	int QueryMed_allLocationleavingPercent(int medID);
	int clearLocationNum(int location);
	int QueryMedleavingPercent( int  medID,int &falg);
	int QueryMedInfo_Client(int location,ClientMedinceInfo * clientmedinfo);
	int getmedID(int locationID,int &medID);//获取药品ID号
	int clearAllLocation(int medId);//清空药品所有地址数量
	int getMaxmedID(int &medID);//获取药品最大ID号
	int deleteMedicine(int med_id);//删除药品详细信息
	int addMedicine (MedInfo &medicineinfo);//新增药品详细信息
	int modifyMedicine(int medID,MedInfo &medicineinfo);//修改药品详细信息
	int lookMedicine(int medID,MedInfo * pmedicineinfo);//查找药品详细信息
	int listSearch(list<MedicinList >* pmedicinList,string medicineName="",  int type =1,BOOL medstate=TRUE);//查找药品列表
	int fastSearch(int type,string input, list<string >* output);//编码快速查找
	int getCurrentMedicineCount(int medID,list<MedinceCount>* medincecount,int flag = 0);//获取药品当前所有剩余的数量
	int addressSearch (int medID,list<MedicinLocationState>*listAddress);//查找药品的地址列表
	int pushMedicinLocation(map<int,MedicinLocationState >&medLocationState);//加药时修改药品位置状态
	int popMedicinLocation(map<int,MedicinLocationState>&medLocationState);//发药时修改药品位置状态
	int modifyMedicinLocation(list<int>location,int falg);//修品药品位置状态
	int getMedicinLocation(map<int,MedicinLocationState> *pmedlocationstate);//输出药品位置状态
	int initializeMedicinLocation();//初始化药品位置状态
	CMedicineManage();
	virtual ~CMedicineManage();
private:
    static map<int,MedicinLocationState>medlocationstate; //药品位置状态(关键字:位置ID)
	static LoggerPtr rootLogger ;
	static bool ini_falg;
	static int m_machinetype;
	static int m_machineID;
	static string max_Muallocation;
	CMutex *mute;
	CDBManage  *db;
};


#endif // !defined(AFX_MEDICINEMANAGE_H__E4947035_86F7_435A_BA68_D46F35BEF8FC__INCLUDED_)
