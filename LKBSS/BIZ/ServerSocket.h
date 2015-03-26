// ServerSocket.h: interface for the CServerSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSOCKET_H__0075F461_91B0_4588_923A_0F460A87587E__INCLUDED_)
#define AFX_SERVERSOCKET_H__0075F461_91B0_4588_923A_0F460A87587E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <winsock2.h>
#include "BIZBaseService.h"
#include "BIZBoxService.h"
#include "Singleton.h"
#include "BIZFillMedicine.h"
#include "MedicineManage.h"
#include "DBManage.h"

//药单信息结构
typedef struct _PrescriptionMed1{
 
	///药品编号
	char medicineId[20];//必须提供

	//药品数量
	char medicineNum[5];//必须提供

	//非整型数量(用于片剂异形)
	char medicineHeteromorphism[5];

	//药品用法及备注
	char medicineHint[50];

	//用药天数
	char useDays[5];//必须提供

	//每天用药次数
	char useTime[5];//必须提供

	//每次用量
	char useQty[5];//必须提供


}PrescriptionMed1;

//处方信息结构
typedef struct _Prescription1{

	//HIS处方编号
	char prescriptionHISId[20];

	//处方类型
	char prescriptionType[5];//0其它
	
	//医生编号
	char prescriptionDoctorId[20];//int 0
	
	//医生姓名
	char prescriptionDoctorName[20];
	
	//处方备注
	char prescriptionHint[50];
	
	//科室编号
	char departmentId[20];//int 0
	
	//科室名称
	char departmentName[30];
	
	//副数
	char fuQty[5];//int 0

	//取药窗口
	char fetchWindow[10];//int 0

}Prescription1;

//病人信息结构
typedef struct _PatientInfo1{

	//诊疗号
	char mzNo[20]; //唯一

	//住院编号
	char inId[20];//int 0

	//床号
	char bedNo[10];//int 0
	
	//患者姓名
	char patientName[20];

	//患者英文名
	char patientNameEn[20];
	
	//患者类型
	char patientType[5];//int 0
	
	//患者备注
	char patientHint[50];
	
	//患者年龄
	char patientAge[5];//int 0
	
	//患者性別
	char patientSex[5];//1为男，2为女
	
}PatientInfo1;

//处方操作结构
typedef struct _PreOperate{
	int preId;//处方号
	int Operatefalg;//操作码
}PreOperate;

//读卡器结构
typedef struct _ReadCard{
	int basketId;//篮子号
	int windowId;//窗口号
}ReadCard;

typedef struct _AddMedinceInfo{
	int medPosition;	//药品位置
	int medNum;			//药品数量
	int oprType;		//操作 0-加药开始，1-加药完成
}AddMedinceInfo;


class CServerSocket  
{
public:
	CServerSocket();
	virtual ~CServerSocket();
	SOCKET m_Listening;
	HANDLE m_hListenThread,m_hAcceptThread;
	static DWORD WINAPI ListenThread(LPVOID lpparam);
	static DWORD WINAPI AcceptThread(LPVOID lpparam);
	int start();
    static bool falg;
	static CString filepath;
	static LoggerPtr rootLogger;
};

#endif // !defined(AFX_SERVERSOCKET_H__0075F461_91B0_4588_923A_0F460A87587E__INCLUDED_)
