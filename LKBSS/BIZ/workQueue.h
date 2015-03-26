#pragma once

#include <list>
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// WorkRec Entities

class CWorkRec{
public:
	CWorkRec (){};
	~CWorkRec(){};

	int getPreId(){
		return this->preId;
	};
	void setPreId(int preId){
		this->preId = preId;
	};

	int getSplitId(){
		return this->splitId;
	};
	void setSplitId(int splitId){
		this->splitId = splitId;
	};

	string getPre_his_id()
	{
		return this->pre_his_id;
	}
    void setPre_his_id(string pre_his_id)
	{
		 this->pre_his_id=pre_his_id;
	}

	int getPreState(){
		return this->preState;
	};
	void setPreState(int preState){
		this->preState = preState;
	};

	int getPreOrder(){
		return this->preOrder;
	};
	void setPreOrder(int preOrder){
		this->preOrder = preOrder;
	};

	int getErrorCode(){
		return this->errorCode;
	};
	void setErrorCode(int errorCode){
		this->errorCode = errorCode;
	};

	string getReason(){
		return this->reason;
	};
	void setReason(string reason){
		this->reason = reason;
	};

	string getCheckInTime(){
		return this->checkInTime;
	};
	void setCheckInTime(string checkInTime){
		this->checkInTime = checkInTime;
	};

	string getLastDealTime(){
		return this->lastDealTime;
	};
	void setLastDealTime(string lastDealTime){
		this->lastDealTime = lastDealTime;
	};
	
	int getFetchState(){
		return this->fetchState;
	};
	void setFetchState(int fetchState){
		this->fetchState = fetchState;
	};

	string getMzNo(){
		return this->mzNo;
	};
	void setMzNo(string mzNo){
		this->mzNo = mzNo;
	};

	string getPatientName(){
		return this->patientName;
	};
	void setPatientName(string patientName){
		this->patientName = patientName;
	};

	string getDepartmentName(){
		return this->departmentName;
	};
	void setDepartmentName(string departmentName){
		this->departmentName = departmentName;
	};

	int getPatientId(){
		return this->patientId;
	};
	void setPatientId(int patientId){
		this->patientId = patientId;
	};
protected:
	int preId;
	int splitId;
	string pre_his_id;
	int preState;
	int preOrder;
	int errorCode;
	string reason;
	string checkInTime;
	string lastDealTime;
	int fetchState;
	string mzNo;
	string patientName;
	string departmentName;
	int patientId;
};

typedef list<CWorkRec> WorkQueue;
