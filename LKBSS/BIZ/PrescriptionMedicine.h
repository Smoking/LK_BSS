#if !defined(PrescriptionMedicine_define)
#define PrescriptionMedicine_define

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrescriptionMedicine.h : header file
//
#include <vector>
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// PrescriptionMedicine Entities

class CPrescriptionMedicine{

public:
	CPrescriptionMedicine()
    {
        medicineId = 0;
        //HIS系统药品编号
        medicineHisId = 0;            
        //药品数量
        medicineNum = 0;

        //实际发要数量
		curSendMedNum = 0;
        
        //处方编号
        prescriptionId = 0;
        
        //用量
        useQty = 0;
        
        //用药天数
        useDays = 0;
        
        //每天用药次数
        useTime = 0; 
        //非整型数量(用于片剂异形)
        medicineHeteromorphism = 0;
        //药品类型
        medicineType = 0;               
        //是否修改,true 修改, false 没有修改
        modified = false;
		
		distState = 0;

		maxNumOnce = 0;
    };
    
	~CPrescriptionMedicine(){};

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
	
	string getMedicineHint(){
		return this->medicineHint;
	}
	void setMedicineHint(string medicineHint){
		this->medicineHint = medicineHint;
	}

	int getMedicineNum(){
		return this->medicineNum;
	}
	void setMedicineNum(int medicineNum){
		this->medicineNum = medicineNum;
	}

	int getCurSendMedNum(){
		return this->curSendMedNum;
	}
	void setCurSendMedNum(int medicineNum){
		this->curSendMedNum = curSendMedNum;
	}

	int getPrescriptionId(){
		return this->prescriptionId;
	}
	void setPrescriptionId(int prescriptionId){
		this->prescriptionId = prescriptionId;
	}

	int getUseQty(){
		return this->useQty;
	}
	void setUseQty(int useQty){
		this->useQty = useQty;
	}

	int getUseDays(){
		return this->useDays;
	}
	void setUseDays(int useDays){
		this->useDays = useDays;
	}

	int getUseTime(){
		return this->useTime;
	}
	void setUseTime(int useTime){
		this->useTime = useTime;
	}

	double getMedicineHeteromorphism(){
		return this->medicineHeteromorphism;
	}
	void setMedicineHeteromorphism(double medicineHeteromorphism){
		this->medicineHeteromorphism = medicineHeteromorphism;
	}

	string getMedicineName(){
		return this->medicineName;
	}
	void setMedicineName(string medicineName){
		this->medicineName = medicineName;
	}
    
    string getMedicineNameEN(){
		return this->medicineNameEN;
	}
	void setMedicineNameEN(string medicineNameEN){
		this->medicineNameEN = medicineNameEN;
	}

	string getMedicineTypeUnit(){
		return this->medicineTypeUnit;
	}
	void setMedicineTypeUnit(string medicineTypeUnit){
		this->medicineTypeUnit = medicineTypeUnit;
	}


	int getMedicineType(){
		return this->medicineType;
	}
	void setMedicineType(int medicineType){
		this->medicineType = medicineType;
	}

    string getMedicineUnit()
    {
        return this->medicineUnit;
    }
    void setMedicineUnit(string medicineUnit)
    {
        this->medicineUnit = medicineUnit;
    }

    bool getModify()
	{
		return modified;
	}
	void setModify(bool bModify = true)
	{
		modified = bModify;
	}

	int getDistState()
	{
		return this->distState;
	}
	void setDistState(int distState)
	{
		this->distState = distState;
	}
    
	int getMaxNumOnce()
	{
		return this->maxNumOnce;
	}
	void setMaxNumOnce(int maxNumOnce)
	{
		this->maxNumOnce = maxNumOnce;
	}

	int getWeight()
	{
		return this->weight;
	}
	void setWeight(int weight)
	{
		this->weight = weight;
	}

	string getMedicineFactoryName()
    {
        return this->medicineFactoryName;
    }
    void setMedicineFactoryName(string medicineFactoryName)
    {
        this->medicineFactoryName = medicineFactoryName;
    }

protected:
	//药品编号
	int medicineId;

    //HIS系统药品编号
	int medicineHisId;
	
	//药品用法及备注
	string medicineHint;
	
	//药品名称
	string medicineName;

    //药品英文名称
    string medicineNameEN;

	//药品摆药单位
	string medicineTypeUnit;

	//药品厂家
	string medicineFactoryName;

	//药品数量
	int medicineNum;

	// 实际发要数量
	int curSendMedNum;

	//处方编号
	int prescriptionId;

	//用量
	int useQty;

	//用药天数
	int useDays;

	//每天用药次数
	int useTime;

	//非整型数量(用于片剂异形)
	double medicineHeteromorphism;

	//药品类型
	int medicineType;

	//药品规格
    string medicineUnit;
	
	//是否修改,true 修改, false 没有修改
	bool modified;

	//发药状态,0正常,1缺药,2其他设备补,3非机储补
	int distState;

	int maxNumOnce; //设备发药最大数量,超过则手工领取

	int weight; //药品权重
};

#endif
