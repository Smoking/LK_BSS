#if !defined(Patient_define)
#define Patient_define

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Patient.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Patient Entities
#include <vector>
#include "Prescription.h"
#include <string>

class CPatient {
	
public:
	CPatient(){};
	~CPatient(){};

	string getPatientId(){
		return this->patientId;
	};
	void setPatientId(string patientId){
		this->patientId = patientId;
	}

	string getPatientName(){
		return this->patientName;
	};
	void setPatientName(string patientName){
		this->patientName = patientName;
	};

	int getPatientType(){
		return this->patientType;
	};
	void setPatientType(int patientType){
		this->patientType = patientType;
	};

	int getPatientState(){
		return this->patientState;
	};
	void setPatientState(int patientState){
		this->patientState = patientState;
	};

	string getCreateTime(){
		return this->createTime;
	};
	void setCreateTime(string createTime){
		this->createTime = createTime;
	};

	string getValidateTime(){
		return this->validateTime;
	};
	void setValidateTime(string createTime){
		this->createTime = createTime;
	};

	string getPatientHint(){
		return this->patientHint;
	};
	void setPatientHint(string patientHint){
		this->patientHint = patientHint;
	};
	
	string getPatientNameEn(){
		return this->patientNameEn;
	};
	void setPatientNameEn(string patientNameEn){
		this->patientNameEn = patientNameEn;
	};

	int getPatientAge(){
		return this->patientAge;
	};
	void setPatientAge(int patientAge){
		this->patientAge = patientAge;
	};

	int getPatientSex(){
		return this->patientSex;
	};
	void setPatientSex(int patientSex){
		this->patientSex = patientSex;
	};

	UINT getInId(){
		return this->inId;
	};
	void setInId(UINT inId){
		this->inId = inId;
	};

	string getBedNo(){
		return this->bedNo;
	};
	void setBedNo(string bedNo){
		this->bedNo = bedNo;
	};

	string getLastDistributeTime(){
		return this->lastDistributeTime;
	};
	void setLastDistributeTime(string lastDistributeTime){
		this->lastDistributeTime = lastDistributeTime;
	};

	string getMzNo(){
		return this->mzNo;
	};
	void setMzNo(string mzNo){
		this->mzNo = mzNo;
	};
	void setAreaNo(string areaNo){
		this->areaNo=areaNo;
	};
	string getAreaNo(){
		return this->areaNo;
	};
	void setDoc_entrust(string doc_entrust){
		this->doc_entrust=doc_entrust;
	};
	string getDoc_entrust(){
		return this->doc_entrust;
	};
protected:
	//患者编号
	string patientId;
	
	//患者姓名
	string patientName;
	
	//患者类型
	int patientType;
	
	//患者状态
	int patientState;
	
	//创建时间
	string createTime;
	
	//有效时间
	string validateTime;
	
	//患者备注
	string patientHint;
	
	//患者英文名
	string patientNameEn;
	
	//患者年龄
	int patientAge;
	
	//患者性別
	int patientSex;

	//住院编号
	UINT inId;

	//床号
	string bedNo;

	//最后发药时间
	string lastDistributeTime;
	//处方
	//vector<CPrescription> preList;
	
	//诊疗号
	string mzNo; 

	//病区号
	string areaNo;

	//医嘱
	string doc_entrust;
};

#endif
