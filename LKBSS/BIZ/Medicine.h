#if !defined(Medicine_define)
#define Medicine_define

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Medicine.h : header file
//
#include "Location.h"
#include <vector>
#include <string>
/////////////////////////////////////////////////////////////////////////////
// Medicine Entities

class CMedicine{

public:
	CMedicine(){};
	~CMedicine(){};
	
	int getMedicineId(){
		return this->medicineId;
	}
	void setMedicineId(int medicineId){
		this->medicineId = medicineId;
	}

	int getMedicineHisId(){
		return this->medicineHisId;
	}
	void setMedicineHisId(int medicineHisId){
		this->medicineHisId = medicineHisId;
	}

	string getMedicineNameCH(){
		return this->medicineNameCH;
	}
	void setMedicineNameCH(string medicineNameCH){
		this->medicineNameCH = medicineNameCH;
	}

	string getMedicineNameEN(){
		return this->medicineNameEN;
	}
	void setMedicineNameEN(string medicineNameEN){
		this->medicineNameEN = medicineNameEN;
	}

	/*string getNumCode(){
		return this->numCode;
	}
	void setNumCode(string numCode){
		this->numCode = numCode;
	}*/

	string getPyCode(){
		return this->pyCode;
	}
	void setPyCode(string pyCode){
		this->pyCode = pyCode;
	}

	/*string getStdCode(){
		return this->stdCode;
	}
	void setStdCode(string stdCode){
		this->stdCode = stdCode;
	}*/

	string getCustCode(){
		return this->custCode;
	}
	void setCustCode(string custCode){
		this->custCode = custCode;
	}

	int getMedicineType(){
		return this->medicineType;
	}
	void setMedicineType(int medicineType){
		this->medicineType = medicineType;
	}

	int getMedicineState(){
		return this->medicineState;
	}
	void setMedicineState(int medicineState){
		this->medicineState = medicineState;
	}

	/*string getMedicineHint(){
		return this->medicineHint;
	}
	void setMedicineHint(string medicineHint){
		this->medicineHint = medicineHint;
	}*/

	string getMedicineUnit(){
		return this->medicineUnit;
	}
	void setMedicineUnit(string medicineUnit){
		this->medicineUnit = medicineUnit;
	}

	int getMaxNumOnce(){
		return this->maxNumOnce;
	}
	void setMaxNumOnce(int maxNumOnce){
		this->maxNumOnce = maxNumOnce;
	}

	int getWarningPercent(){
		return this->warningPercent;
	}
	void setWarningPercent(int warningPercent){
		this->warningPercent = warningPercent;
	}

	/*int getMedUnitCapacity(){
		return this->medUnitCapacity;
	}
	void setMedUnitCapacity(int medUnitCapacity){
		this->medUnitCapacity = medUnitCapacity;
	}*/
	
protected:
	//ҩƷ���
	int medicineId;

	//HISϵͳҩƷ���
	int medicineHisId;
	
	//ҩƷ��������
	string medicineNameCH;

	//ҩƷӢ������
	string medicineNameEN;

	//��ݱ���
	//string numCode;  //����
	string pyCode;  //ƴ��
	//string stdCode; //��׼
	string custCode; //�û��Զ���

	//ҩƷ����(�м�,Ƭ����)
	int medicineType;

	//ҩƷ״̬
	int medicineState;

	//ҩƷ��ע
	//string medicineHint;

	//ҩƷ���
	string medicineUnit;

	//�����������
	int maxNumOnce;

	//��������
	int warningPercent;

	//�����
	//int medUnitCapacity;
};


#endif
