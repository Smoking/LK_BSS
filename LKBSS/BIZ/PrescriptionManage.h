

#if !defined(PrescriptionManage_define)
#define PrescriptionManage_define


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Prescription.h"
#include "PrescriptionMedicine.h"
#include "Patient.h"
#include "DBManage.h"
#include <string>
#include <list>
#include <map>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include "ErrorCodeRes.h"

using namespace log4cxx;

/*
//处方查询条件
struct PrescriptionFilter
{
	string prescriptionHISId;	//HIS处方编号
	string departmentName;		//科室名称
	string patientName;			//患者姓名
	int prescriptionSource;		//处方来源
	DWORD	beginTime;			//开始时间 
	DWORD	endTime;			//结束时间
};
*/

/* 操作码及意义
1301	新增处方
1302	删除处方
1303	删除处方药品
1304 	作废处方
1305	修改处方
1306	修改处方药品信息
1307	修改处方病人信息
*/

//处方信息
struct PrescriptionInfo
{
	CPrescription prescription;					//处方
	CPatient prePatient;						//病人信息
	list<CPrescriptionMedicine> preMedicine;	//药品列表
};

class CPrescriptionManager
{
public:
	CPrescriptionManager();
	~CPrescriptionManager();
	long inputPrescription(PrescriptionInfo  &prescriptionInfo);		//录入处方信息
	//bool inputMedicine(const CPrescriptionMedicine &prescriptionMed);	//录入处方药品
	//bool inputPatient(const CPatient &PrescriptionPatient);					//录入处方病人信息
	//查询处方
	long queryPrescription(map<string,string> &queryWhere, list<PrescriptionInfo> &prescriptionInfo, string extraText="");
	long getPatient(int prescriptionId, CPatient &patient);	//取得病人信息
	long getPrescriptionMedicine(int prescriptionId, list<CPrescriptionMedicine> &preMedicine ,int splitId=0);  //取得处方药品列表	
	long modifyPrescription(int prescriptionId, map<string,string> &prescription); //修改处方
	//long modifyMedicine(int prescriptionId, int medicineId, map<string,string> &medicine);		//修改药品
	long modifyPresMedicine(int prescriptionId, list<CPrescriptionMedicine> &preMedicine);		//修改药品
	long modifyPatient(string patientId, map<string,string> &patient);			//修改病人，map的第一个值为要修改的字段，第二个值为此字段值
	long deletePrescription(int prescriptionId);					//删除处方
	long deleteMedicine(int prescriptionId, int medicineId);		//删除药品
	long deletePatient(int patientId);								//删除病人信息
	long listLastPrescriptionMedicine(list<CPrescriptionMedicine> &preMedicine, int num, string strWhere="");  //列出常用药品清单
    long blankOutPrescription(CPrescription &prescription, long &prescriptionId);  //作废处方
    string getBasketId(string cardId);
private:
	static LoggerPtr rootLogger;
	CDBManage *db;
	
	long savePatient(CPatient &prePatient, string &patientId);		//保存病人
	long savePrescriptionMedicine(long prescriptionId, list<CPrescriptionMedicine> &preMedicine);  //保存药品
	void getPatientFields(CPatient &prePatient);		//设置处方病人信息
	long getPrescriptionMedicineFields(string sqlText, list<CPrescriptionMedicine> &listPreMedicine);	//取得处方药品信息
    long savePrescription(CPrescription &prescription, long &prescriptionId);  //保存处方
};

#endif
