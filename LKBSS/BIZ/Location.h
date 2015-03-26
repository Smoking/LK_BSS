#if !defined(Location_define)
#define Location_define

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Location.h : header file
//
#include <string>
/////////////////////////////////////////////////////////////////////////////
// Location Entities
using namespace std;

class CLocation{
public:
	CLocation(){};
	~CLocation(){};

	int getLocationId(){
		return this->locationId;
	};
	void setLocationId(int locationId){
		this->locationId = locationId;
	};

	int getMaxCapacity(){
		return this->maxCapacity;
	};
	void setMaxCapacity(int maxCapacity){
		this->maxCapacity = maxCapacity;
	};

	int getCurQuantity(){
		return this->curQuantity;
	};
	void setCurQuantity(int curQuantity){
		this->curQuantity = curQuantity;
	};

	int getLocationState(){
		return this->locationState;
	};
	void setLocationState(int locationState){
		this->locationState = locationState;
	};

	int getLocationType(){
		return this->locationType;
	};
	void setLocationType(int locationType){
		this->locationType = locationType;
	};

	int getMedicineId(){
		return this->medicineId;
	};
	void setMedicineId(int medicineId){
		this->medicineId = medicineId;
	};

	int getMachineId(){
		return this->machineId;
	};
	void setMachineId(int machineId){
		this->machineId = machineId;
	};

	int getWarningPercent(){
		return this->warningPercent;
	};
	void setWarningPercent(int warningPercent){
		this->warningPercent = warningPercent;
	};
	int getLightPosition(){
		return this->lightPosition;
	};
	void setLightPosition(int lightPosition){
		this->lightPosition = lightPosition;
	};
	
	int getUnitNum(){
		return (1+(locationId-1)/320);
	};

	int getLayerNum(){
		int layerNum = (locationId-1)%320;
		layerNum = (layerNum)/32 +1;
		return layerNum;
	};

	int getPositionNum(){
		int positionNum =(locationId-1)%32 + 1;
		return positionNum;
	};
	
	bool operator > ( const CLocation& A )const
	{
		return curQuantity > A.curQuantity;
	}

	bool operator < ( const CLocation& A )const
	{
		return curQuantity < A.curQuantity;
	}
protected:
	//λ�ñ���
	int locationId;

	//�������
	int maxCapacity;

	//��ǰ����
	int curQuantity;

	//λ��״̬
	int locationState;

	//λ������
	int locationType;

	//����ҩƷ���
	int medicineId;
	
	//�������
	int machineId;

	//�������
	int warningPercent;
	
	//led�Ƶ�ַ
	int lightPosition;
	
};

#endif
