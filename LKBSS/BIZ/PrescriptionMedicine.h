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
        //HISϵͳҩƷ���
        medicineHisId = 0;            
        //ҩƷ����
        medicineNum = 0;

        //ʵ�ʷ�Ҫ����
		curSendMedNum = 0;
        
        //�������
        prescriptionId = 0;
        
        //����
        useQty = 0;
        
        //��ҩ����
        useDays = 0;
        
        //ÿ����ҩ����
        useTime = 0; 
        //����������(����Ƭ������)
        medicineHeteromorphism = 0;
        //ҩƷ����
        medicineType = 0;               
        //�Ƿ��޸�,true �޸�, false û���޸�
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
	//ҩƷ���
	int medicineId;

    //HISϵͳҩƷ���
	int medicineHisId;
	
	//ҩƷ�÷�����ע
	string medicineHint;
	
	//ҩƷ����
	string medicineName;

    //ҩƷӢ������
    string medicineNameEN;

	//ҩƷ��ҩ��λ
	string medicineTypeUnit;

	//ҩƷ����
	string medicineFactoryName;

	//ҩƷ����
	int medicineNum;

	// ʵ�ʷ�Ҫ����
	int curSendMedNum;

	//�������
	int prescriptionId;

	//����
	int useQty;

	//��ҩ����
	int useDays;

	//ÿ����ҩ����
	int useTime;

	//����������(����Ƭ������)
	double medicineHeteromorphism;

	//ҩƷ����
	int medicineType;

	//ҩƷ���
    string medicineUnit;
	
	//�Ƿ��޸�,true �޸�, false û���޸�
	bool modified;

	//��ҩ״̬,0����,1ȱҩ,2�����豸��,3�ǻ�����
	int distState;

	int maxNumOnce; //�豸��ҩ�������,�������ֹ���ȡ

	int weight; //ҩƷȨ��
};

#endif
