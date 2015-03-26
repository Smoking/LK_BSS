#if !defined(BatchInfo_define)
#define BatchInfo_define

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BatchInfo_define.h : header file
//

#include <string>

using namespace std;


/////////////////////////////////////////////////////////////////////////////
// User Entities

class CBatchInfo {
	
public:
	CBatchInfo(){};
	~CBatchInfo(){};

	int getBatchId(){
		return this->batchId;
	};
	void setBatchId(int batchId){
		this->batchId = batchId;
	};

	int getLocationId(){
		return this->locationId;
	};
	void setLocationId(int locationId){
		this->locationId = locationId;
	};

	int getMedId(){
		return this->medId;
	};
	void setMedId(int medId){
		this->medId = medId;
	};
	
	string getProduceDate(){
		return this->produceDate;
	};
	void setProduceDate(string produceDate){
		this->produceDate = produceDate;
	};

	string getBatchNo(){
		return this->batchNo;
	};
	void setBatchNo(string batchNo){
		this->batchNo = batchNo;
	};

	string getValidateDate(){
		return this->validateDate;
	};
	void setValidateDate(string validateDate){
		this->validateDate = validateDate;
	};


protected:
	//���ű��
	int batchId;

	//��ַ���
	int locationId;

	//ҩƷ���
	int medId;

	//��������
	string produceDate;

	//����
	string batchNo;

	//��Ч����
	string validateDate;

};

#endif
