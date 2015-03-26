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
	//���߱��
	string patientId;
	
	//��������
	string patientName;
	
	//��������
	int patientType;
	
	//����״̬
	int patientState;
	
	//����ʱ��
	string createTime;
	
	//��Чʱ��
	string validateTime;
	
	//���߱�ע
	string patientHint;
	
	//����Ӣ����
	string patientNameEn;
	
	//��������
	int patientAge;
	
	//�����Ԅe
	int patientSex;

	//סԺ���
	UINT inId;

	//����
	string bedNo;

	//���ҩʱ��
	string lastDistributeTime;
	//����
	//vector<CPrescription> preList;
	
	//���ƺ�
	string mzNo; 

	//������
	string areaNo;

	//ҽ��
	string doc_entrust;
};

#endif
