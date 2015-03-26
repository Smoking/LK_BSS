#if !defined(WorkflowRecord_define)
#define WorkflowRecord_define

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkflowRecord.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// WorkflowRecord Entities

class CWorkFlowRecord
{
public:
	CWorkFlowRecord(){};
	~CWorkFlowRecord(){};

	int getOperId(){
		return this->operId;
	}
	void setOperId(int operId){
		this->operId = operId;
	}

	int getOperCode(){
		return this->operCode;
	}
	void setOperCode(int operCode){
		this->operCode = operCode;
	}

	int getUserId(){
		return this->userId;
	}
	void setUserId(int userId){
		this->userId = userId;
	}

	int getPreId(){
		return this->preId;
	}
	void setPreId(int preId){
		this->preId = preId;
	}
	int getMedId(){
		return this->medId;
	}
	void setMedId(int medId){
		this->medId = medId;
	}
	int getMedNum(){
		return this->medNum;
	}
	void setMedNum(int medNum){
		this->medNum = medNum;
	}
	
	DWORD getOperTime(){
		return this->operTime;
	}
	void setOperTime(DWORD operTime){
		this->operTime = operTime;
	}

	string getOperHint(){
		return this->operHint;
	}
	void setOperHint(string operHint){
		this->operHint = operHint;
	}
	
protected:
	int operId;
	int operCode;
	int userId;
	int preId;
	int medId;
	int medNum;
	DWORD operTime;
	string operHint;
	
};
#endif
