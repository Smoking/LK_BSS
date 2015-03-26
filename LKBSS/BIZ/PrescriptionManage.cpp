

#include "stdafx.h"
#include "PrescriptionManage.h"
#include "Utils.h"
#include "ConfigUtil.h"

//using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;       
#endif

/////////////////////////////////////////////////////////////////////////////
//ERROR CODE 
/*
#define ER_NO_PATIENT_INFO				130001	//没有患者信息
#define ER_NO_MEDICINE_INFO				130002	//没有药品信息
#define ER_SAVE_PRESCRIPTION			130003	//保存处方失败
#define ER_SAVE_PATIENT					130004	//保存病人失败
#define ER_SAVE_PRESCRIPTION_MEDICINE	130005	//保存药品失败
#define ER_INPUT_PRESCRIPTION			130006	//录入处方失败
#define ER_QUERY_PRESCRIPTION			130007	//处方查询失败
#define ER_GET_PATIENT					130008	//获取病人信息失败
#define ER_GET_PRESCRIPTION_MEDICINE	130009	//获取处方药品信息失败
#define ER_MODIFY_PRESCRIPTION			130010	//修改处方失败
#define ER_MODIFY_PRESCRIPTION_MEDICINE	130011	//修改处方药品信息失败
#define ER_MODIFY_PRESCRIPTION_PATIENT	130012	//修改处方病人信息失败
#define ER_DEL_PRESCRIPTION				130013	//删除处方失败
#define ER_DEL_PRESCRIPTION_MEDICINE	130014	//删除处方药品信息失败
#define ER_DEL_PATIENT					130015	//删除病人信息失败
//
*/
/////////////////////////////////////////////////////////////////////////////

LoggerPtr CPrescriptionManager::rootLogger = Logger::getLogger("CPrescriptionManager");

CPrescriptionManager::CPrescriptionManager()
{
	try{
	
	//rootLogger->trace("function CPrescriptionManager start..............");
	string msg = configUtil.getValue("logPropFile");
	PropertyConfigurator::configure(msg);
	db = new CDBManage();
	//rootLogger->trace("function CPrescriptionManager end..............");
	}catch (...) {
		rootLogger->fatal("CPrescriptionManager--CPrescriptionManager error ");
	}
}

CPrescriptionManager::~CPrescriptionManager()
{
	try{
	
	//rootLogger->trace("function ~CPrescriptionManager start..............");
	//db->close();
	delete db;
	//rootLogger->trace("function ~CPrescriptionManager end..............");
	}catch (...) {
		rootLogger->fatal("CPrescriptionManager--~CPrescriptionManager error ");
	}
}

/************************************************************************/
/* 保存药品清单，失败返回错误码，成功返回0
/* preMedicine：药品信息清单
/* prescriptionId：处方号
   bModify : 是否为修改模式， true 修改， false 新建

   2009.12.10 增加bModify
/* 2009.9.3
/************************************************************************/
long CPrescriptionManager::savePrescriptionMedicine(long prescriptionId, list<CPrescriptionMedicine> &preMedicine)
{
	try{
	
	//rootLogger->trace("function savePrescriptionMedicine start..............");
	string sqlText;
	long ret = 0;	
	
	list<CPrescriptionMedicine>::iterator plist;	
	for(plist=preMedicine.begin(); plist!=preMedicine.end(); plist++)
	{	
		sqlText = "insert into prescription_medicine_list ("
				  "pre_id,"
				  "med_id,"
				  "med_num,"
				  //"med_heteromorphism,"
				  "med_hint,"
				  "use_qty,"
				  "use_days,"
				  "use_time"
				  ") values ("
				  + Utils::itos(prescriptionId) + ","
				  + Utils::itos(plist->getMedicineId()) + ","
				  + Utils::itos(plist->getMedicineNum()) + ","
				  //+ Utils::dtos(plist->getMedicineHeteromorphism()) + ","
				  + "'" + plist->getMedicineHint() + "',"
				  + Utils::itos(plist->getUseQty()) + ","
				  + Utils::itos(plist->getUseDays()) + ","
				  + Utils::itos(plist->getUseTime())
				  + ")";
		ret = db->execSQL(sqlText);
		if (ret != 0)
		{
			rootLogger->error("failed to save prescription medicine, errorCode: " + Utils::itos(ret));
			return ER_SAVE_PRESCRIPTION_MEDICINE;
		}
	}
	//rootLogger->trace("function savePrescriptionMedicine end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--savePrescriptionMedicine error.");
		rootLogger->fatal("CPrescriptionManager--savePrescriptionMedicine error ");
		return -1;
	}
}

/************************************************************************/
/* 保存病人，失败返回错误码，成功返回0
/* prePatient：病人信息
   patientId: 病人Id (out)

/* 2009.9.2
/************************************************************************/
long CPrescriptionManager::savePatient(CPatient &prePatient, string &patientId)		//保存病人
{
	try{
	
	//rootLogger->trace("function savePatient start..............");
	long ret=0;
	string mzNo = prePatient.getMzNo();
	if (!mzNo.empty())  //病人信息中有门诊号（诊疗号）
	{
		string sqlText = "select patient_id from patient where mz_no = '" + mzNo + "'";
		ret = db->execSQL(sqlText);
		if (ret != 0)
		{
			return ret;
		}
		
		if (db->getRowNum() != 0)  //存在此门诊号的病人
		{   
			patientId = db->getFieldValueByIndex(0);
			//goto tabEnd;
			//rootLogger->trace("function savePatient end..............");
			return 0;
		}
	}

	string sqlText = "insert into patient ("
					"patient_type," //--tinyint(4) DEFAULT NULL, 
					//"patient_state," //--tinyint(4) DEFAULT NULL,
					"patient_name," //-- varchar(20) CHARACTER SET latin1 DEFAULT NULL,
					"patient_name_en," //-- varchar(20) CHARACTER SET latin1 DEFAULT NULL,
					//-- `validate_time`, -- datetime DEFAULT NULL,
					"create_time,"
					"patient_age," //--smallint(6) DEFAULT NULL,
					"patient_sex,"  //-- tinyint(4) DEFAULT NULL,
					"patient_hint,"  //-- varchar(200) CHARACTER SET latin1 DEFAULT NULL,
					//--- `last_distribute_time`, //-- datetime DEFAULT NULL,
					"In_id," //-- int(11) DEFAULT NULL,
					"bed_no"; //-- varchar(10) CHARACTER SET latin1 DEFAULT NULL,
	if (!mzNo.empty())
	{
		sqlText += ",mz_no"; //"mz_no" //--varchar(20) DEFAULT NULL,
	}					
	sqlText	= sqlText + ") values("			
					+ Utils::itos(prePatient.getPatientType()) + ","
					//+ Utils::itos(prePatient.getPatientState()) + ","
					+ "'" + prePatient.getPatientName() + "',"
					+ "'" + prePatient.getPatientNameEn() + "',"
					+ "now(),"
					+ Utils::itos(prePatient.getPatientAge()) + ","
					+ Utils::itos(prePatient.getPatientSex()) + ","
					+ "'" + prePatient.getPatientHint() + "',"
					+ Utils::itos(prePatient.getInId()) + ","
					+ "'" + prePatient.getBedNo();
	if (!mzNo.empty())
	{
		sqlText = sqlText + "','" + prePatient.getMzNo();
	}	
	sqlText +="')";	

	ret = db->execSQL(sqlText);
	if (ret != 0)
	{
		rootLogger->error("failed to save patient, errorCode: " + Utils::itos(ret));
		return ER_SAVE_PATIENT;
	}	
	patientId = Utils::itos(db->getInsertId());	

//tabEnd:
	//rootLogger->trace("function savePatient end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--savePatient error.");
		rootLogger->fatal("CPrescriptionManager--savePatient error ");
		return -1;
	}
}

/************************************************************************/
/* 保存处方，失败返回错误码，成功返回0
/* prescription：处方
/* prescriptionId：保存至数据库后处方Id(out)

/* 2009.9.2
/************************************************************************/
long CPrescriptionManager::savePrescription(CPrescription &prescription, long &prescriptionId)  
{
	try{
	
	//rootLogger->trace("function savePrescription start..............");
	string sqlText;
	sqlText = "insert into prescription (";
	string doctorId = prescription.getPrescriptionDoctorId();
	if (!doctorId.empty())
	{
		sqlText += "doctor_id,";
	}
	string departmentId = prescription.getDepartmentId();
	if (!departmentId.empty())
	{
		sqlText += "department_id,";
	}		
	sqlText += "pre_type,"
			  "pre_state,"
			  "doctor_name,"
			  "patient_id,"
			  "pre_source,"
			  "pre_hint,"
			  "department_name,"
			  "fu_qty,"
			  "pre_priority,"
			  "fetch_window,"
			  "create_time,"
			  "validate_time,"
			  "pre_his_id,"
			  "pre_old_id)"
			  "values("; 
			  
	if (!doctorId.empty())
	{
		sqlText = sqlText + doctorId + ",";
	}
	if (!departmentId.empty())
	{
		sqlText = sqlText + departmentId + ",";
	}

	sqlText += Utils::itos(prescription.getPrescriptionType()) + ","
			  + Utils::itos(prescription.getPrescriptionState()) + ","
			  + "'" + prescription.getPrescriptionDoctorName() + "',"
			  + prescription.getPatientId() + ","
			  + Utils::itos(prescription.getPrescriptionSource()) + ","
			  + "'" + prescription.getPrescriptionHint() + "',"
			  + "'" + prescription.getDepartmentName() + "',"
			  //+ Utils::itos(prescription.getFuQty()) + ","
			  + "1,"
			  + Utils::itos(prescription.getPrescriptionPriority()) + ","
			  + "'" + prescription.getFetchWindow() + "',"
			  + "now(),'"
			  + prescription.getValidateTime() + "',"
			  + "'" + prescription.getprescriptionHISId() + "',"
			  + Utils::itos(prescription.getPrescriptionOldId()) + ")";
	long ret;
	ret = db->execSQL(sqlText);
	if (ret != 0)
	{
		rootLogger->error("failed to save prescription, errorCode: " + Utils::itos(ret));
		rootLogger->error("sql = " + sqlText);
		return ER_SAVE_PRESCRIPTION;
	}
	prescriptionId = db->getInsertId();
	rootLogger->debug("new prescriptionId: " + Utils::itos(prescriptionId));
	//rootLogger->trace("function savePrescription end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--savePrescription error.");
		rootLogger->fatal("CPrescriptionManager--savePrescription error ");
		return -1;
	}
}

/************************************************************************/
/* 录入处方信息，失败返回错误码，成功返回0
/* prescriptionInfo：处方信息
/* 2010.7.20
   增加记日志功能
/* 2009.9.3
/************************************************************************/
long CPrescriptionManager::inputPrescription(PrescriptionInfo &prescriptionInfo)		//录入处方信息
{
	try{
	
	//rootLogger->trace("function inputPrescription start..............");
	/*												
	string mzNo = prescriptionInfo.prePatient.getMzNo();
	if (mzNo != "")  //his传来的病人信息中有门诊号（诊疗号）
	{
		string sqlText = "select patient_id from patient where mz_no = '" + mzNo + "'";
		ret = db->execSQL(sqlText);
		if (ret != 0)
		{
			return ret;
		}
		if (db->getRowNum() == 0)  //不存在此门诊号的病人
		{
			ret = savePatient(prescriptionInfo.prePatient);
			if (ret != 0)
			{
				rootLogger->error("failed to input prescription, errorCode:" + Utils::itos(ret));
				return ER_INPUT_PRESCRIPTION;
			}
		}
	}*/

	if (prescriptionInfo.preMedicine.size() == 0)
	{
		rootLogger->error("no medicine information");
		return ER_NO_MEDICINE_INFO;
	}

	long ret = 0;
	string patientId = "";  //病人ID
	ret = savePatient(prescriptionInfo.prePatient, patientId);
	if (ret != 0)
	{
		rootLogger->error("failed to input prescription, errorCode:" + Utils::itos(ret));
		//return ER_INPUT_PRESCRIPTION;
        return ret;
	}
	if (patientId == "")
	{
		rootLogger->error("no patient information");
		return ER_NO_PATIENT_INFO;
	}
	
	long preId = 0; //处方ID
	prescriptionInfo.prescription.setPatientId(patientId);
	ret = savePrescription(prescriptionInfo.prescription, preId);  //保存处方 
	if (ret != 0)
	{
		rootLogger->error("failed to input prescription, errorCode:" + Utils::itos(ret));
		return ER_INPUT_PRESCRIPTION;
	}

    string rsLog;
    rsLog = "新增处方号=";
    rsLog += Utils::itos(preId);
    Utils::insertSystemlog("1301", "新增处方", rsLog);
	ret = savePrescriptionMedicine(preId, prescriptionInfo.preMedicine);  //保存药品 
	if (ret != 0)
	{
		rootLogger->error("failed to input prescription, errorCode:" + Utils::itos(ret));
		//return ER_INPUT_PRESCRIPTION;
        return ret;
	}

	//rootLogger->trace("function inputPrescription end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--inputPrescription error.");
		rootLogger->fatal("CPrescriptionManager--inputPrescription error ");
		return -1;
	}
}

/************************************************************************/
/* 取得病人信息各字段值
/* prePatient：病人信息(out)

/* 2009.9.4
/************************************************************************/
void CPrescriptionManager::getPatientFields(CPatient &prePatient)	
{
	try{
	
	//rootLogger->trace("function setPatient start..............");
	//prePatient.setPatientId(atoi(db->getFieldValueByName("patient_id").c_str()));		
	prePatient.setPatientId(db->getFieldValueByName("patient_id"));		
	prePatient.setPatientName(db->getFieldValueByName("patient_name"));			
	prePatient.setPatientType(atoi(db->getFieldValueByName("patient_type").c_str()));			
	prePatient.setPatientState(atoi(db->getFieldValueByName("patient_state").c_str()));			
	prePatient.setCreateTime(db->getFieldValueByName("create_time"));			
	prePatient.setValidateTime(db->getFieldValueByName("validate_time"));			
	prePatient.setPatientHint(db->getFieldValueByName("patient_hint"));			
	prePatient.setPatientNameEn(db->getFieldValueByName("patient_name_en"));			
	prePatient.setPatientAge(atoi(db->getFieldValueByName("patient_age").c_str()));			
	prePatient.setPatientSex(atoi(db->getFieldValueByName("patient_sex").c_str()));	
	//int i = atoi(db->getFieldValueByName("In_id").c_str());
	prePatient.setInId(atoi(db->getFieldValueByName("In_id").c_str()));			
	//prePatient.setInId(i);			
	prePatient.setBedNo(db->getFieldValueByName("bed_no"));			
	prePatient.setLastDistributeTime(db->getFieldValueByName("last_distribute_time"));			
	prePatient.setMzNo(db->getFieldValueByName("mz_no"));
	//prePatient.setAreaNo(db->getFieldValueByName("area_no"));
	//prePatient.setDoc_entrust(db->getFieldValueByName("patient_hint"));
	//rootLogger->trace("function setPatient end..............");
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--setPatient error.");
		rootLogger->fatal("CPrescriptionManager--setPatient error ");
	}
}

/************************************************************************/
/* 查询处方，失败返回错误码，成功返回0
/* queryWhere：查询条件
/* prescriptionInfo：处方信息列表(out)

/* 2009.9.4
/************************************************************************/
long CPrescriptionManager::queryPrescription(map<string,string> &queryWhere,list<PrescriptionInfo> &prescriptionInfo,  string extraText)
{
	try{
	
	//rootLogger->trace("function queryPrescription start..............");
	string sqlText;

	try
	{
		sqlText = "select * from prescription, patient where prescription.patient_id = patient.patient_id";
		map<string, string>::iterator iter;	
		for(iter = queryWhere.begin(); iter != queryWhere.end(); iter++)		
		{
			sqlText += " and " + iter->first + "='" + iter->second + "'";
		}
		if (extraText != "")
		{
			sqlText += extraText;
		}
        sqlText += " order by prescription.create_time desc";
		/*
		if (!queryWhere.prescriptionHISId.empty())
		{
			sqlText += " and pre_his_id='" + queryWhere.prescriptionHISId + "'";
		}
		if (!queryWhere.departmentName.empty())
		{
			sqlText += " and department_name='" + queryWhere.departmentName + "'";
		}
		if (!queryWhere.patientName.empty())
		{
			sqlText += " and patient_name='" + queryWhere.patientName + "'";
		}
		if (!(queryWhere.prescriptionSource == -1))
		{
			sqlText += "and pre_source=" + Utils::itos(queryWhere.prescriptionSource);
		}
		//if () 开始时间 
		//{
		//}
*/		
		long lRet;
		lRet = db->execSQL(sqlText);
		if (lRet != 0)
		{
			rootLogger->error("failed to query prescription, errorCode:" + Utils::itos(lRet));
			return ER_QUERY_PRESCRIPTION;
		}
		
		while (!db->isEnd())
		{
			PrescriptionInfo preInfo;
			preInfo.prescription.setprescriptionHISId(db->getFieldValueByName("pre_his_id"));
			preInfo.prescription.setPrescriptionDoctorId(db->getFieldValueByName("doctor_id"));
			preInfo.prescription.setPrescriptionDoctorName(db->getFieldValueByName("doctor_name"));
			preInfo.prescription.setPrescriptionHint(db->getFieldValueByName("pre_hint"));
			preInfo.prescription.setDepartmentId(db->getFieldValueByName("department_id"));
			preInfo.prescription.setDepartmentName(db->getFieldValueByName("department_name"));
			preInfo.prescription.setFetchWindow(db->getFieldValueByName("fetch_window"));			
			preInfo.prescription.setPrescriptionId(atoi(db->getFieldValueByName("pre_id").c_str()));
			preInfo.prescription.setPrescriptionType(atoi(db->getFieldValueByName("pre_type").c_str()));
			preInfo.prescription.setPrescriptionState(atoi(db->getFieldValueByName("pre_state").c_str()));
			//preInfo.prescription.setPatientId(atoi(db->getFieldValueByName("patient_id").c_str()));
			preInfo.prescription.setPatientId(db->getFieldValueByName("patient_id"));
			preInfo.prescription.setPrescriptionSource(atoi(db->getFieldValueByName("pre_source").c_str()));
			preInfo.prescription.setFuQty(atoi(db->getFieldValueByName("fu_qty").c_str()));
			preInfo.prescription.setPrescriptionPriority(atoi(db->getFieldValueByName("pre_priority").c_str()));
			preInfo.prescription.setErrorCode(atoi(db->getFieldValueByName("error_code").c_str()));
			preInfo.prescription.setCreateTime(db->getFieldValueByName("create_time"));
			preInfo.prescription.setValidateTime(db->getFieldValueByName("validate_time"));	
			preInfo.prescription.setFinishTime(db->getFieldValueByName("finish_time"));	
			preInfo.prescription.setPrescriptionOldId(atoi(db->getFieldValueByName("pre_old_id").c_str()));	

			/*
			preInfo.prePatient.setPatientId(atoi(db->getFieldValueByName("patient_id").c_str()));		
			preInfo.prePatient.setPatientName(db->getFieldValueByName("patient_name"));			
			preInfo.prePatient.setPatientType(atoi(db->getFieldValueByName("patient_type").c_str()));			
			preInfo.prePatient.setPatientState(atoi(db->getFieldValueByName("patient_state").c_str()));			
			preInfo.prePatient.setCreateTime(db->getFieldValueByName("create_time"));			
			preInfo.prePatient.setValidateTime(db->getFieldValueByName("validate_time"));			
			preInfo.prePatient.setPatientHint(db->getFieldValueByName("patient_hint"));			
			preInfo.prePatient.setPatientNameEn(db->getFieldValueByName("patient_name_en"));			
			preInfo.prePatient.setPatientAge(atoi(db->getFieldValueByName("patient_age").c_str()));			
			preInfo.prePatient.setPatientSex(atoi(db->getFieldValueByName("patient_sex").c_str()));			
			preInfo.prePatient.setInId(atoi(db->getFieldValueByName("In_id").c_str()));			
			preInfo.prePatient.setBedNo(db->getFieldValueByName("bed_no"));			
			preInfo.prePatient.setLastDistributeTime(db->getFieldValueByName("last_distribute_time"));			
			preInfo.prePatient.setMzNo(db->getFieldValueByName("mz_no"));
			*/
			getPatientFields(preInfo.prePatient);
			prescriptionInfo.push_back(preInfo);
			db->nextRow();
		}
		db->freeRecord();
	}catch (CException *pEx) 
	{
		rootLogger->error("failed to query prescription");
		pEx->Delete();
		return ER_QUERY_PRESCRIPTION;
	}
	//rootLogger->trace("function queryPrescription end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--queryPrescription error.");
		rootLogger->fatal("CPrescriptionManager--queryPrescription error ");
		return -1;
	}
}

/************************************************************************/
/* 取得处方药品信息
	sqlText: sql语句
	preMedicine：处方药品信息(out)
	2010.1.4	增加获取med_unit
	2009.12.3
/*	2009.9.4
/************************************************************************/
long CPrescriptionManager::getPrescriptionMedicineFields(string sqlText, list<CPrescriptionMedicine> &listPreMedicine)
{
	try{
	
	//rootLogger->trace("function getPrescriptionMedicineFields start..............");
	try
	{
		long lRet = db->execSQL(sqlText);
		if (lRet != 0)
		{
			rootLogger->error("failed to get prescription medicine, errorCode:" + Utils::itos(lRet));
			return ER_GET_PRESCRIPTION_MEDICINE;
		}
		
		CPrescriptionMedicine preMed;
		while (!db->isEnd())
		{
			//getPrescriptionMedicineFields(preMed);
			preMed.setMedicineName(db->getFieldValueByName("med_name_ch"));
            preMed.setMedicineNameEN(db->getFieldValueByName("med_name_en"));
			preMed.setMedicineTypeUnit(db->getFieldValueByName("type_unit"));
			preMed.setMedicineType(atoi(db->getFieldValueByName("med_type").c_str()));
            preMed.setMedicineUnit(db->getFieldValueByName("med_unit"));
			preMed.setMedicineFactoryName(db->getFieldValueByName("factory_name"));
			preMed.setMedicineId(atoi(db->getFieldValueByName("med_id").c_str()));
            preMed.setMedicineHisId(atoi(db->getFieldValueByName("med_his_id").c_str()));
			preMed.setMedicineHint(db->getFieldValueByName("med_hint"));
			preMed.setMedicineNum(atoi(db->getFieldValueByName("med_num").c_str()));
			preMed.setPrescriptionId(atoi(db->getFieldValueByName("pre_id").c_str()));
			preMed.setUseQty(atoi(db->getFieldValueByName("use_qty").c_str()));
			preMed.setUseDays(atoi(db->getFieldValueByName("use_days").c_str()));
			preMed.setUseTime(atoi(db->getFieldValueByName("use_time").c_str()));
			preMed.setMedicineHeteromorphism(atof(db->getFieldValueByName("med_heteromorphism").c_str()));	
			preMed.setMaxNumOnce(atoi(db->getFieldValueByName("max_num_once").c_str()));
			preMed.setWeight(atoi(db->getFieldValueByName("weight").c_str()));
			preMed.setDistState(atoi(db->getFieldValueByName("dist_state").c_str()));

			listPreMedicine.push_back(preMed);
			db->nextRow();
		}
		db->freeRecord();
	}catch (CException *pEx) 
	{
		rootLogger->error("failed to query prescription");
		pEx->Delete();
		return ER_GET_PRESCRIPTION_MEDICINE;
	}


	/*
	preMedicine.setMedicineName(db->getFieldValueByName("med_name_ch"));
	preMedicine.setMedicineTypeUnit(db->getFieldValueByName("type_unit"));
	preMedicine.setMedicineId(atoi(db->getFieldValueByName("med_id").c_str()));
	preMedicine.setMedicineHint(db->getFieldValueByName("med_hint"));
	preMedicine.setMedicineNum(atoi(db->getFieldValueByName("med_num").c_str()));
	preMedicine.setPrescriptionId(atoi(db->getFieldValueByName("pre_id").c_str()));
	preMedicine.setUseQty(atoi(db->getFieldValueByName("use_qty").c_str()));
	preMedicine.setUseDays(atoi(db->getFieldValueByName("use_days").c_str()));
	preMedicine.setUseTime(atoi(db->getFieldValueByName("use_time").c_str()));
	preMedicine.setMedicineHeteromorphism(atof(db->getFieldValueByName("med_heteromorphism").c_str()));
	*/
	//rootLogger->trace("function getPrescriptionMedicineFields end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--getPrescriptionMedicineFields error.");
		rootLogger->fatal("CPrescriptionManager--getPrescriptionMedicineFields error ");
		return -1;
	}
}


/************************************************************************/
/* 取得处方药品列表，失败返回错误码，成功返回0
/* prescriptionId：处方号
/* preMedicine：处方药品列表(out)

   2010.1.4	增加获取med_unit
/* 2009.9.4
/************************************************************************/
long CPrescriptionManager::getPrescriptionMedicine(int prescriptionId, list<CPrescriptionMedicine> &preMedicine ,int splitId)
{
	try{
	
	//rootLogger->trace("function getPrescriptionMedicine start..............");
	string sqlText;
    if(splitId == 0)
	{
		sqlText = "select *"  
			" from prescription_medicine_list medList left join medicine med on med.med_id = medList.med_id"
			" where medList.pre_id = " + Utils::itos(prescriptionId);

	}
	else
	{

		sqlText = "select *"  
			" from prescription_split_medicine_list medList left join medicine med on med.med_id = medList.med_id"
			" where medList.pre_id = " + Utils::itos(prescriptionId);
		sqlText += " and  split_id = " + Utils::itos(splitId);
	}

	//rootLogger->trace("function getPrescriptionMedicine end..............");
	return getPrescriptionMedicineFields(sqlText, preMedicine);
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--getPrescriptionMedicine error.");
		rootLogger->fatal("CPrescriptionManager--getPrescriptionMedicine error ");
		return -1;
	}
}

/************************************************************************/
/* 取得病人信息，失败返回错误码，成功返回0
/* prescriptionId：处方号
/* patient：病人信息(out)

/* 2009.9.4
/************************************************************************/
long CPrescriptionManager::getPatient(int prescriptionId, CPatient &patient)	//取得病人信息
{
	try{
	
	//rootLogger->trace("function getPatient start..............");
	string sqlText;
	sqlText = "select * from prescription, patient"
		" where prescription.patient_id = patient.patient_id and pre_id = " + Utils::itos(prescriptionId);
	long lRet = db->execSQL(sqlText);
	if (lRet != 0)
	{
		rootLogger->error("failed to get patient, errorCode:" + Utils::itos(lRet));
		return ER_GET_PATIENT;
	}
	if (db->getRowNum() != 0)
	{
		getPatientFields(patient);
	}
	//rootLogger->trace("function getPatient end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--getPatient error.");
		rootLogger->fatal("CPrescriptionManager--getPatient error ");
		return -1;
	}
}

/************************************************************************/
/* 修改处方,map的第一个值为要修改的字段，第二个值为此字段值，失败返回错误码，成功返回0
/* prescriptionId：处方号
/* prescription：处方修改字段信息
/************************************************************************/
long CPrescriptionManager::modifyPrescription(int prescriptionId, map<string,string> &prescription)
{
	try{
	
	//rootLogger->trace("function modifyPrescription start..............");
	string strWhere("where pre_id=");
	strWhere += Utils::itos(prescriptionId);
	long lRet = db->updateTable("prescription", strWhere, prescription);
	if (lRet != 0)
	{
		rootLogger->error("failed to modify prescription, errorCode: " + Utils::itos(lRet));
		return ER_MODIFY_PRESCRIPTION;
	}
    string rsLog = "处方号=";
    rsLog = rsLog + Utils::itos(prescriptionId);
    Utils::insertSystemlog("1305", "修改处方", rsLog);
	//rootLogger->trace("function modifyPrescription end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--modifyPrescription error.");
		rootLogger->fatal("CPrescriptionManager--modifyPrescription error ");
		return -1;
	}
}	

/************************************************************************/
/* 修改处方药品,失败返回错误码，成功返回0
/* prescriptionId：处方号
   preMedicine: 处方药品列表
/************************************************************************/
long CPrescriptionManager::modifyPresMedicine(int prescriptionId, list<CPrescriptionMedicine> &preMedicine)
{
	try{
	
	//rootLogger->trace("function modifyPresMedicine start..............");

	string sqlText;
	string strPreId, strMedId;
	long ret = 0;	
	
	list<CPrescriptionMedicine>::iterator plist;	
	for(plist=preMedicine.begin(); plist!=preMedicine.end(); plist++)
	{
		strPreId = Utils::itos(prescriptionId);
		strMedId = Utils::itos(plist->getMedicineId());
		if (plist->getModify())
		{
			sqlText = "select pre_id from prescription_medicine_list where pre_id=" + strPreId + " and med_id=" + strMedId;
			if (db->execSQL(sqlText))
			{
				rootLogger->error("failed to modify prescription medicine, errorCode: " + Utils::itos(ret));
				return ER_MODIFY_PRESCRIPTION_MEDICINE;	
			}
			if (db->getRowNum())
			{
				sqlText = "update prescription_medicine_list set "
						"med_num=" + Utils::itos(plist->getMedicineNum()) + ","
						"med_hint=" + "'" + plist->getMedicineHint() + "',"
						"use_qty=" + Utils::itos(plist->getUseQty()) + ","
						"use_days=" + Utils::itos(plist->getUseDays()) + ","
						"use_time=" + Utils::itos(plist->getUseTime())
						+ " where pre_id=" + strPreId + " and med_id=" + strMedId;				
			} else
			{
				sqlText = "insert into prescription_medicine_list ("
						"pre_id,"
						"med_id,"
						"med_num,"
						//"med_heteromorphism,"
						"med_hint,"
						"use_qty,"
						"use_days,"
						"use_time"
						") values ("
						+ Utils::itos(prescriptionId) + ","
						+ Utils::itos(plist->getMedicineId()) + ","
						+ Utils::itos(plist->getMedicineNum()) + ","
						//+ Utils::dtos(plist->getMedicineHeteromorphism()) + ","
						+ "'" + plist->getMedicineHint() + "',"
						+ Utils::itos(plist->getUseQty()) + ","
						+ Utils::itos(plist->getUseDays()) + ","
						+ Utils::itos(plist->getUseTime())
						+ ")";
			}	
			ret = db->execSQL(sqlText);
			
			if (ret != 0)
			{
				rootLogger->error("failed to modify prescription medicine, errorCode: " + Utils::itos(ret));
				return ER_MODIFY_PRESCRIPTION_MEDICINE;
			}
			
		}
		/*string strWhere("where pre_id='");
		strWhere += prescriptionId + "' and med_id='";
		strWhere +=  medicineId + "'";
		long lRet = db->updateTable("prescription_medicine_list", strWhere, medicine);
		if (lRet != 0)
		{
		rootLogger->error("failed to modify prescription medicine, errorCode: " + Utils::itos(lRet));
		return ER_MODIFY_PRESCRIPTION_MEDICINE;
		}*/
	}
    string rsLog = "处方号=";
    rsLog = rsLog + Utils::itos(prescriptionId);
    Utils::insertSystemlog("1306", "修改处方药品信息", rsLog);
	//rootLogger->trace("function modifyPresMedicine end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--modifyPresMedicine error.");
		rootLogger->fatal("CPrescriptionManager--modifyPresMedicine error ");
		return -1;
	}
}

/************************************************************************/
/* 修改处方病人信息,map的第一个值为要修改的字段，第二个值为此字段值，失败返回错误码，成功返回0
/* patientId：病人号
/* patient：处方病人修改字段信息
/************************************************************************/
long CPrescriptionManager::modifyPatient(string patientId, map<string,string> &patient)			//修改病人
{
	try{
	
	//rootLogger->trace("function modifyPatient start..............");
	string strWhere("where patient_id='");
	strWhere += patientId + "'";
	long lRet = db->updateTable("patient", strWhere, patient);
	if (lRet != 0)
	{
		rootLogger->error("failed to modify prescription patient, errorCode: " + Utils::itos(lRet));
		return ER_MODIFY_PRESCRIPTION_PATIENT;
	}
    string rsLog = "病人编号=";
    rsLog = rsLog + patientId;
    Utils::insertSystemlog("1307", "修改处方病人信息", rsLog);
	//rootLogger->trace("function modifyPatient end..............");	
	return 0;
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--modifyPatient error.");
		rootLogger->fatal("CPrescriptionManager--modifyPatient error ");
		return -1;
	}
}


/************************************************************************/
/* 删除处方,失败返回错误码，成功返回0
/* prescriptionId：处方号
/************************************************************************/
long CPrescriptionManager::deletePrescription(int prescriptionId)
{
	try{
	
	//rootLogger->trace("function deletPrescription start..............");
	string sqlText = "delete from prescription where pre_id=" + Utils::itos(prescriptionId);
	long lRet = db->execSQL(sqlText);
	if (lRet != 0)
	{
		rootLogger->error("failed to delete prescription, errorCode: " + Utils::itos(lRet));
		return ER_DEL_PRESCRIPTION;
	}
    string rsLog = "删除处方号=";
    rsLog += Utils::itos(prescriptionId);
    Utils::insertSystemlog("1302", "删除处方", rsLog);
	//rootLogger->trace("function deletPrescription end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--deletPrescription error.");
		rootLogger->fatal("CPrescriptionManager--deletPrescription error ");
		return -1;
	}
}

/************************************************************************/
/* 删除处方药品,失败返回错误码，成功返回0
/* prescriptionId：处方号
/* medicineId：药品号
/************************************************************************/
long CPrescriptionManager::deleteMedicine(int prescriptionId, int medicineId)
{
	try{
	
	//rootLogger->trace("function deleteMedicine start..............");
	string sqlText = "delete from prescription_medicine_list where pre_id=" + Utils::itos(prescriptionId) 
					+ " and med_id=" + Utils::itos(medicineId);
	long lRet = db->execSQL(sqlText);
	if (lRet != 0)
	{
		rootLogger->error("failed to delete prescription medicine, errorCode: " + Utils::itos(lRet));
		return ER_DEL_PRESCRIPTION_MEDICINE;
	}
    string rsLog = "处方号=";
    rsLog = rsLog + Utils::itos(prescriptionId) + "药品编号=" + Utils::itos(medicineId);
    Utils::insertSystemlog("1303", "删除处方药品", rsLog);
	//rootLogger->trace("function deleteMedicine end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--deleteMedicine error.");
		rootLogger->fatal("CPrescriptionManager--deleteMedicine error ");
		return -1;
	}
}

/************************************************************************/
/* 删除病人信息,失败返回错误码，成功返回0
/* patientId：病人号
/* 
/************************************************************************/
long CPrescriptionManager::deletePatient(int patientId)
{
	try{
	
	//rootLogger->trace("function deletePatient start..............");
	string sqlText = "delete from patient where patient_id=" + Utils::itos(patientId);
	long lRet = db->execSQL(sqlText);
	if (lRet != 0)
	{
		rootLogger->error("failed to delete patient, errorCode: " + Utils::itos(lRet));
		return ER_DEL_PATIENT;
	}	
    string rsLog = "病人编号=";
    rsLog = rsLog + Utils::itos(patientId);
    Utils::insertSystemlog("1303", "删除病人", rsLog);
	//rootLogger->trace("function deletePatient end..............");
	return 0;
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--deletePatient error.");
		rootLogger->fatal("CPrescriptionManager--deletePatient error ");
		return -1;
	}
}								

/************************************************************************/
/* 列出常用药品清单,失败返回错误码，成功返回0
/* preMedicine：处方药品列表(out)
/* num: 列出记录数
/************************************************************************/
long CPrescriptionManager::listLastPrescriptionMedicine(list<CPrescriptionMedicine> &preMedicine, int num, string strWhere)
{
	try{
	
	//rootLogger->trace("function listLastPrescriptionMedicine start..............");
	string sqlText = "select *"  //med.med_name_ch,med.type_unit,med.med_type,medList.*"
					" from `prescription_medicine_list` medList,prescription pre,medicine med"
					" where medList.pre_id = pre.pre_id and medList.med_id = med.med_id"
					" group by med.med_id"
					" order by pre.create_time desc limit " + Utils::itos(num);
	
	//rootLogger->trace("function listLastPrescriptionMedicine end..............");
	return getPrescriptionMedicineFields(sqlText, preMedicine);
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--listLastPrescriptionMedicine error.");
		rootLogger->fatal("CPrescriptionManager--listLastPrescriptionMedicine error ");
		return -1;
	}
}

/************************************************************************/
/* 作废处方，失败返回错误码，成功返回0
/* prescription：处方
/* prescriptionId：保存至数据库后处方Id(out)

/************************************************************************/
long CPrescriptionManager::blankOutPrescription(CPrescription &prescription, long &prescriptionId)  //作废处方
{
	try{
	
	//rootLogger->trace("function blankOutPrescription begin..............");
    long lRet = savePrescription(prescription, prescriptionId);
    if (lRet)
    {
        return lRet;
    }
    string rsLog = "作废后处方号=";
    rsLog = rsLog + Utils::itos(prescriptionId);
    Utils::insertSystemlog("1304", "作废处方", rsLog);
 
    //rootLogger->trace("function blankOutPrescription end..............");
    return 0;
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--blankOutPrescription error.");
		rootLogger->fatal("CPrescriptionManager--blankOutPrescription error ");
		return -1;
	}
}

/************************************************************************/
/* 通过卡号获取篮子号, 返回篮子号
/* cardId: 篮子上的卡号
/*

/************************************************************************/
string CPrescriptionManager::getBasketId(string cardId)   
{
	try{
    //rootLogger->trace("function getBasketId begin..............");
    string sBasketId = "";
    string sqlText = "select basket_id from basket_mapping where card_id1='" + cardId + "' or card_id2='" + cardId + "'";
    long lRet = db->execSQL(sqlText);
    if (lRet==0 && db->getRowNum()>0)
    { 
        sBasketId = db->getFieldValueByIndex(0);
    }
	else
	{   
		rootLogger->error(sqlText);
		rootLogger->error("getBasketId errorCode = "+Utils::itos(lRet));
	}
    return sBasketId;
	}catch (...) {
		/// AfxMessageBox("CPrescriptionManager--getBasketId error.");
		rootLogger->fatal("CPrescriptionManager--getBasketId error ");
		return "";
	}
}
