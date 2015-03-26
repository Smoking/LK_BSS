#if !defined(BIZBoxService_define)
#define BIZBoxService_define

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BIZBoxService.h : header file
//

#include "BIZBaseService.h"
#include "Utils.h"

class CBIZBoxService : public CBIZBaseService{

private:
	static LoggerPtr rootLogger ;
	int distributing(CWorkRec workRec,CDBManage* cdb);   // ·¢Ò©²Ù×÷
	void ThreadEntryPoint();
	int bindBasket(string basketId,int preId,int splitId,CDBManage* cdb);
	int remapLocation(list<CPrescriptionMedicine> listMedicine, 
						map<int,MedicinSort>& msMap,
						map<int,LocationStatistic>& lsMap,
						list<MedicineInfo>& medList,
						map<int,MedicinLocationState>& medLocMap,
						CDBManage* cdb);
	int allocateWindows(int preId,int splitId,CDBManage* cdb, int basketNo);
	int updateWindowBasketCount(int num ,int windowNo,CDBManage* cdb);
	int getWindowNo(int preId,int splitId,int& windowNo,CDBManage* cdb);
public:
	CBIZBoxService();
	~CBIZBoxService();
};

#endif
