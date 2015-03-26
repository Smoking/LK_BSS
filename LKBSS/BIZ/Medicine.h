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
	//药品编号
	int medicineId;

	//HIS系统药品编号
	int medicineHisId;
	
	//药品中文名称
	string medicineNameCH;

	//药品英文名称
	string medicineNameEN;

	//快捷编码
	//string numCode;  //数字
	string pyCode;  //拼音
	//string stdCode; //标准
	string custCode; //用户自定义

	//药品类型(盒剂,片剂等)
	int medicineType;

	//药品状态
	int medicineState;

	//药品备注
	//string medicineHint;

	//药品规格
	string medicineUnit;

	//单包最大数量
	int maxNumOnce;

	//警告数量
	int warningPercent;

	//规格量
	//int medUnitCapacity;
};


#endif
