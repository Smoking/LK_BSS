#if !defined(Prescription_define)
#define Prescription_define

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Prescription.h : header file
//
#include <vector>
#include "PrescriptionMedicine.h"
#include <string>
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// Prescription Entities

class CPrescription
{
public:
    CPrescription()
    {
        prescriptionId = 0;
        prescriptionType = 0;
        prescriptionState = 0;
        prescriptionSource = 0;
        fuQty = 0;
        prescriptionPriority = 0;
        errorCode = 0;
        prescriptionOldId = 0;
    };
    
	~CPrescription(){};
 
	string getprescriptionHISId(){
		return this->prescriptionHISId;
	}
	
	void setprescriptionHISId(string preHISId)
	{
		prescriptionHISId = preHISId;
	}

	int getPrescriptionId(){
		return this->prescriptionId;
	}
	void setPrescriptionId(int prescriptionId){
		this->prescriptionId = prescriptionId;
	}
	
	int getPrescriptionType(){
		return this->prescriptionType;
	}
	void setPrescriptionType(int prescriptionType){
		this->prescriptionType = prescriptionType;
	}

	int getPrescriptionState(){
		return this->prescriptionState;
	}
	void setPrescriptionState(int prescriptionState){
		this->prescriptionState = prescriptionState;
	}

	string getPatientId(){
		return this->patientId;
	}
	void setPatientId(string patientId){
		this->patientId = patientId;
	}

	int getPrescriptionSource(){
		return this->prescriptionSource;
	}
	void setPrescriptionSource(int prescriptionSource){
		this->prescriptionSource = prescriptionSource;
	}

	int getFuQty(){
		return this->fuQty;
	}
	void setFuQty(int fuQty){
		this->fuQty = fuQty;
	}

	string getPrescriptionDoctorId(){
		return this->prescriptionDoctorId;
	}
	void setPrescriptionDoctorId(string prescriptionDoctorId){
		this->prescriptionDoctorId = prescriptionDoctorId;
	}

	string getPrescriptionDoctorName(){
		return this->prescriptionDoctorName;
	}
	void setPrescriptionDoctorName(string prescriptionDoctorName){
		this->prescriptionDoctorName = prescriptionDoctorName;
	}

	string getPrescriptionHint(){
		return this->prescriptionHint;
	}
	void setPrescriptionHint(string prescriptionHint){
		this->prescriptionHint = prescriptionHint;
	}
	
	string getDepartmentId(){
		return this->departmentId;
	}
	void setDepartmentId(string departmentId){
		this->departmentId = departmentId;
	}

	string getDepartmentName(){
		return this->departmentName;
	}
	void setDepartmentName(string departmentName){
		this->departmentName = departmentName;
	}

	int getPrescriptionPriority(){
		return this->prescriptionPriority;
	}
	void setPrescriptionPriority(int prescriptionPriority){
		this->prescriptionPriority = prescriptionPriority;
	}

	string getFetchWindow(){
		return this->fetchWindow;
	}
	void setFetchWindow(string fetchWindow){
		this->fetchWindow = fetchWindow;
	}

	int getErrorCode(){
		return this->errorCode;
	}
	void setErrorCode(int errorCode){
		this->errorCode = errorCode;
	}


	string getCreateTime(){
		return this->createTime;
	}
	void setCreateTime(string createTime){
		this->createTime = createTime;
	}

	string getValidateTime(){
		return this->validateTime;
	}
	void setValidateTime(string validateTime){
		this->validateTime = validateTime;
	}
	
	string getFinishTime(){
		return this->finishTime;
	}
	void setFinishTime(string finishTime){
		this->finishTime = finishTime;
	}

	int getPrescriptionOldId()
	{
		return prescriptionOldId;
	}
	void setPrescriptionOldId(int preOldId)
	{
		prescriptionOldId = preOldId;
	}
	
protected:
	//HIS处方编号
	string prescriptionHISId;

	//处方编号
	int prescriptionId;

	//处方类型
	int prescriptionType;
	
	//处方状态
	int prescriptionState;
	
	//医生编号
	string prescriptionDoctorId;
	
	//医生姓名
	string prescriptionDoctorName;
	
	//患者编号
	string patientId;
	
	//处方来源
	int prescriptionSource;
	
	//处方备注
	string prescriptionHint;
	
	//科室编号
	string departmentId;
	
	//科室名称
	string departmentName;
	
	//副数
	int fuQty;

	//优先级
	int prescriptionPriority;

	//取药窗口
	string fetchWindow;

	//错误编码
	int errorCode;

	//创建时间
	string createTime;


	//有效时间
	string validateTime;

	//发药完成时间
	string finishTime;
	
	//作废前处方ID
	int prescriptionOldId;

};

#endif
