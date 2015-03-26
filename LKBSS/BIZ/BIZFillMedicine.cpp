
#include "stdafx.h"
#include "BIZFillMedicine.h"
#include "ConfigUtil.h"
#include "Utils.h"
LoggerPtr CBIZFillMedicine::rootLogger = Logger::getLogger("CBIZFillMedicine");
//list<int> CBIZFillMedicine::runningList;
vector<FillStruct> CBIZFillMedicine::vfs;
list<FillStruct> CBIZFillMedicine::fillList;

CBIZFillMedicine::CBIZFillMedicine()
{
	try{
	
	//rootLogger->trace("CBIZFillMedicine construct begin............");
	this->autoNext = true;
	
		//初始化日志记录器
	string msg = configUtil.getValue("logPropFile");
	PropertyConfigurator::configure(msg);
    pBoxSinglechip=singletonBoxSinglechip.getInst();
	cMedicineManage = new CMedicineManage();
	locationName = CMedicineManage::locationName;
    max_Muallocation = configUtil.getInt("MualAddMedUnit")*320 ;
	//rootLogger->trace("CBIZFillMedicine construct end................");
	}catch (...) {
		/// AfxMessageBox("CBIZFillMedicine--CBIZFillMedicine error.");
		rootLogger->fatal("CBIZFillMedicine--CBIZFillMedicine error ");
	}
}

CBIZFillMedicine::~CBIZFillMedicine(){
	try{
	
	//rootLogger->trace("destroy begin................");
	if( pBoxSinglechip != NULL)
	{
		singletonBoxSinglechip.releaseInst(pBoxSinglechip);
	}
	delete cMedicineManage;
	//rootLogger->trace("destroy end....................");
	}catch (...) {
		/// AfxMessageBox("CBIZFillMedicine--~CBIZFillMedicine error.");
		rootLogger->fatal("CBIZFillMedicine--~CBIZFillMedicine error ");
	}
}

void CBIZFillMedicine::setHwnd(HWND hwnd){
	this->hwnd = hwnd;
}

list<CLocation> CBIZFillMedicine::dispatchMedicine(CMedicine medcine){

	try{
	
	//rootLogger->trace("function dispatchMedicine begin....................");
	list<CLocation> cloclist;
	CLocation tmp;

	list<MedicinLocationState> locList;
	MedicinLocationState tmp2;

	int result = cMedicineManage->addressSearch(medcine.getMedicineId(),&locList);

	list<MedicinLocationState>::iterator ptr = locList.begin();

	
	while (ptr != locList.end())
	{
		tmp2 = *ptr;
		tmp.setLocationId(tmp2.location_id);
		tmp.setCurQuantity(tmp2.cur_quantity);
		tmp.setLocationState(tmp2.location_state);
		tmp.setLocationType(tmp2.location_type);
		tmp.setMachineId(tmp2.machine_id);
		tmp.setMaxCapacity(tmp2.max_capacity);
		tmp.setMedicineId(tmp2.medicine_id);
		tmp.setWarningPercent(tmp2.warning_percent);

		cloclist.push_back(tmp);
		ptr++;
	}
	
	//rootLogger->trace("function dispatchMedicine end......................");
	return cloclist;
	}catch (...) {
		rootLogger->fatal("CBIZFillMedicine--dispatchMedicine error ");
		list<CLocation> cloclist;
		return cloclist;
	}
}
int CBIZFillMedicine::initFilling(){
	try{
	
	//rootLogger->trace("function initFilling start............................");
	list<FillStruct>::iterator ptr = fillList.begin();

	vfs.clear();
	for (int i = 0; i <= 3 ; i++)
	{
		if (ptr == fillList.end())
		{
			break;
		}
		FillStruct fs = *ptr;
		vfs.push_back(fs);
//		vfs[i] = tmpFS;
		ptr++;
		fillList.pop_front();

	}
	//rootLogger->trace("function initFIlling end.............................");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CBIZFillMedicine--fillMedicine error.");
		rootLogger->fatal("CBIZFillMedicine--fillMedicine error ");
		return -1;
	}
}
int CBIZFillMedicine::fillMedicine(int order){

	try{
	
	//rootLogger->trace("function fillMedicine start.........................");
	
	if (vfs[order].locationId == 0)
	{
		rootLogger->warn("the order "+Utils::itos(order) + " is empty!");
		return 0;
	}
	FillStruct fs = vfs[order];
	MedicineInfo mi;
	mi.position = fs.lightPosition;
	mi.deviceNumber = fs.machineId;

	int errorCode = pBoxSinglechip->fillMedicineStartBox(mi);
	if(errorCode != 0)
	{
		rootLogger->error("device error !");
		
		return errorCode;
	}

	//rootLogger->trace("function fillMedicine end.........................");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CBIZFillMedicine--fillMedicine error.");
		rootLogger->fatal("CBIZFillMedicine--fillMedicine error ");
		return -1;
	}
}

int CBIZFillMedicine::finishFilling(int count ,int order){
	try{
	
	//rootLogger->trace("function finishFilling start.................");
	if (order >= vfs.size())
	{
		return 0;
	}
	
	FillStruct fs = vfs[order];

	MedicineInfo mi;
	mi.position = fs.lightPosition;
	mi.deviceNumber = fs.machineId;
	
	if (fs.locationId == 0 )
	{
		return 0;
	}
	int errorCode = pBoxSinglechip->fillMedicineFinishBox(mi);
	if(errorCode != 0)
	{
		rootLogger->error("device error !");
		return errorCode;
	}
	
	map<int,MedicinLocationState> medLocMap;
	MedicinLocationState medLoc;

	medLoc.cur_quantity = count ;
	medLoc.location_id = fs.locationId;
	medLocMap.insert(pair<int,MedicinLocationState>(medLoc.location_id,medLoc));

	errorCode = cMedicineManage->pushMedicinLocation(medLocMap);
	if(errorCode != 0)
	{
		rootLogger->error("write location state error location = "+Utils::itos(medLoc.location_id));
		return errorCode;
	}
	
	list<FillStruct>::iterator ptr = fillList.begin();
	if (ptr != fillList.end())
	{
		vfs[order] = *ptr;
		fillList.pop_front();
	}else{
		vfs[order].locationId = 0;
	}

	rootLogger->info("fillMedicine succeed location = "+Utils::itos(medLoc.location_id)+", num = "+Utils::itos(count));

	//rootLogger->trace("function finishFilling end...........................");

	return 0;
	}catch (...) {
		/// AfxMessageBox("CBIZFillMedicine--finishFilling error.");
		rootLogger->fatal("CBIZFillMedicine--finishFilling error ");
		return -1;
	}
}
int CBIZFillMedicine::sendLocationToQueue(list<CLocation> locationList,bool clearList){
	try{
	
	//rootLogger->trace("function sendLocationToQueue begin................");
	FillStruct tmp;
	CLocation cloc; 
	list<CLocation>::iterator ptr ;

	if (clearList)
	{
		this->fillList.clear();
	}
	
	for (ptr = locationList.begin(); ptr != locationList.end();ptr++)
	{
		cloc = *ptr;
		if(cloc.getLocationState()==0 || cloc.getLocationId() > max_Muallocation)
			continue;
		tmp.locationId = cloc.getLocationId();
		tmp.machineId = cloc.getMachineId();
		tmp.medId = cloc.getMedicineId();
		tmp.num = cloc.getCurQuantity();
		tmp.lightPosition = cloc.getLocationId();

		fillList.push_back(tmp);
	}

	//rootLogger->trace("function sendLocationToQueue end.....................");
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZFillMedicine--sendLocationToQueue error ");
		return -1;
	}
}

int CBIZFillMedicine::sendMedicineToQueue(list<CMedicine> medicineList){
	try{
	
	//rootLogger->trace("function sendMedicineToQueue begin................");
	list<CMedicine>::iterator ptr ;

	list<CLocation> cloclist;

	this->fillList.clear();
	
	for (ptr = medicineList.begin();ptr != medicineList.end();ptr++)
	{
		cloclist.clear();
		cloclist = this->dispatchMedicine(*ptr);
		
		this->sendLocationToQueue(cloclist,false);
	}

	//rootLogger->trace("function sendMedicineToQueue end................");
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZFillMedicine--sendMedicineToQueue error ");
		return -1;
	}
}

int CBIZFillMedicine::skipLocation(){
	try{
	
	//rootLogger->trace("function skipLocation begin.......................");
	if (!fillList.empty())
	{
		fillList.pop_front();
		return 0;
	}

	return FILLLIST_EMPTY;
	
	//rootLogger->trace("function skipLocation end........................");
	}catch (...) {
		rootLogger->fatal("CBIZFillMedicine--skipLocation error ");
		return -1;
	}
}

int CBIZFillMedicine::skipMedicine(int& n){
	try{
	
	//rootLogger->trace("function skipMedicine begin .......................");

	n = 0;
	if (fillList.empty())
	{
		return FILLLIST_EMPTY;
	}
	list<FillStruct>::iterator ptr;
	ptr=fillList.begin();

	int medId = (*ptr).medId;

	fillList.pop_front();
	n++;

	ptr=fillList.begin();
	
	while (ptr!=fillList.end())
	{
		if((*ptr).medId == medId)
		{
			fillList.pop_front();
			ptr = fillList.begin();
			n++;
		}else{
			break;
		}
	}

	//rootLogger->trace("function skipMedicine END...............................");
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZFillMedicine--skipMedicine error ");
		return -1;
	}
}

int CBIZFillMedicine::getValidateLocationId(){
	try{
	
	//rootLogger->trace("function getValidateLocationId begin........................");
	FillStruct tmp;
	list<FillStruct>::iterator ptr = fillList.begin();

	if (ptr != fillList.end())
	{
		tmp = *ptr;
		return tmp.locationId;
	}

	//rootLogger->trace("function getValidateLocationId end..............................");
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZFillMedicine--getValidateLocationId error ");
		return -1;
	}
}


list<FillStruct> CBIZFillMedicine::getFillList(){

	return this->fillList;
}

LocDisplay CBIZFillMedicine::getLocationDetail(int locationId,CDBManage* cdb)
{
	try{	
	//rootLogger->trace("function getLocationDetail begin....................");
	LocDisplay loc;
	if (locationId == 0)
	{
		return loc;
	}
	string sql = "select t1.location_id,t1.medicine_id,t1.cur_quantity,t1.max_capacity,t2.med_name_ch,t2.med_unit ";
	sql +=" from "+locationName +" t1,medicine t2 where t1.medicine_id = t2.med_id and t1.location_id = ";
	sql += Utils::itos(locationId);

	rootLogger->info("sql = "+sql);
	
	int result = cdb->execSQL(sql);
	
	if(!cdb->isEnd())
	{
		loc.curCapacity = atoi(cdb->getFieldValueByName("cur_quantity").c_str());
		loc.locationId = atoi(cdb->getFieldValueByName("location_id").c_str());
		loc.maxCapacity = atoi(cdb->getFieldValueByName("max_capacity").c_str());
		loc.medicineId = atoi(cdb->getFieldValueByName("medicine_id").c_str());
		loc.medicineName = cdb->getFieldValueByName("med_name_ch");
		loc.medcineUnit = cdb->getFieldValueByName("med_unit");
	}	
	cdb->freeRecord();
	//rootLogger->trace("function getLocationDetail end........................");
	return loc;
	}catch (...) {
		rootLogger->fatal("CBIZFillMedicine--getLocationDetail error ");
		LocDisplay loc;
		return loc;
	}
}



vector<FillStruct> CBIZFillMedicine::getVfs()
{
	return this->vfs;
}

int CBIZFillMedicine::fillMedicineRemote(int locationId)
{
	try{
	
	//rootLogger->trace("function fillMedicineRemote start.........................");
	
	if (locationId == 0)
	{
		rootLogger->warn("location id is empty!");
		return 0;
	}
	/*else
	{
		list<int>::iterator ptr = find_if(runningList.begin(), runningList.end(),FindById(locationId));
		if (ptr != runningList.end())
		{
			rootLogger->warn("location is used already");
			return 2;  // 该地址正在被操作
		}else{
			runningList.push_back(locationId);
		}
	}*/

	MedicineInfo mi;
	mi.position = locationId;
	mi.deviceNumber = 0;

	int errorCode = pBoxSinglechip->fillMedicineStartBox(mi);
	if(errorCode != 0)
	{
		rootLogger->error("device error !");
		//runningList.pop_back();
		return errorCode;
	}

	//rootLogger->trace("function fillMedicineRemote end.........................");
	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZFillMedicine--fillMedicineRemote error ");
		return -1;
	}
}

int CBIZFillMedicine::finishFillingRemote(int count,int locationId,CDBManage* cdb)
{
	try{
	
	//rootLogger->trace("function finishFillingRemote start.................");
	
	if (locationId == 0)
	{
		rootLogger->warn("location id is empty!");
		return 0;
	}
	/*else
	{
		list<int>::iterator ptr = find_if(runningList.begin(), runningList.end(),FindById(locationId));
		if (ptr == runningList.end())
		{
			rootLogger->warn("location is not used!!");
			return 2;  // 该地址从未开始加药
		}else{
			runningList.remove(*ptr);
		}
	}*/

	LocDisplay locInfo = this->getLocationDetail(locationId,cdb);
	
	MedicineInfo mi;
	mi.position = locationId;
	mi.deviceNumber = 0;

	int errorCode = pBoxSinglechip->fillMedicineFinishBox(mi);
	if(errorCode != 0)
	{
		rootLogger->error("device error !");
		//runningList.push_back(locationId);
		return errorCode;
	}
	
	map<int,MedicinLocationState> medLocMap;
	MedicinLocationState medLoc;

	if (locInfo.curCapacity + count > locInfo.maxCapacity)
	{
		medLoc.cur_quantity = locInfo.maxCapacity - locInfo.curCapacity;
	}else
	{
		medLoc.cur_quantity = count ;
	}
	
	medLoc.location_id = locationId;
	medLocMap.insert(pair<int,MedicinLocationState>(medLoc.location_id,medLoc));

	errorCode = cMedicineManage->pushMedicinLocation(medLocMap);
	if(errorCode != 0)
	{
		//runningList.push_back(locationId);
		return errorCode;
	}
	//rootLogger->trace("function finishFillingRemote end...........................");

	return 0;
	}catch (...) {
		rootLogger->fatal("CBIZFillMedicine--finishFillingRemote error ");
		return -1;
	}
}
