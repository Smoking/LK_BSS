

#if !defined(PrescriptionManage_define)
#define PrescriptionManage_define


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Prescription.h"
#include "PrescriptionMedicine.h"
#include "Patient.h"
#include "DBManage.h"
#include <string>
#include <list>
#include <map>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include "ErrorCodeRes.h"

using namespace log4cxx;

/*
//������ѯ����
struct PrescriptionFilter
{
	string prescriptionHISId;	//HIS�������
	string departmentName;		//��������
	string patientName;			//��������
	int prescriptionSource;		//������Դ
	DWORD	beginTime;			//��ʼʱ�� 
	DWORD	endTime;			//����ʱ��
};
*/

/* �����뼰����
1301	��������
1302	ɾ������
1303	ɾ������ҩƷ
1304 	���ϴ���
1305	�޸Ĵ���
1306	�޸Ĵ���ҩƷ��Ϣ
1307	�޸Ĵ���������Ϣ
*/

//������Ϣ
struct PrescriptionInfo
{
	CPrescription prescription;					//����
	CPatient prePatient;						//������Ϣ
	list<CPrescriptionMedicine> preMedicine;	//ҩƷ�б�
};

class CPrescriptionManager
{
public:
	CPrescriptionManager();
	~CPrescriptionManager();
	long inputPrescription(PrescriptionInfo  &prescriptionInfo);		//¼�봦����Ϣ
	//bool inputMedicine(const CPrescriptionMedicine &prescriptionMed);	//¼�봦��ҩƷ
	//bool inputPatient(const CPatient &PrescriptionPatient);					//¼�봦��������Ϣ
	//��ѯ����
	long queryPrescription(map<string,string> &queryWhere, list<PrescriptionInfo> &prescriptionInfo, string extraText="");
	long getPatient(int prescriptionId, CPatient &patient);	//ȡ�ò�����Ϣ
	long getPrescriptionMedicine(int prescriptionId, list<CPrescriptionMedicine> &preMedicine ,int splitId=0);  //ȡ�ô���ҩƷ�б�	
	long modifyPrescription(int prescriptionId, map<string,string> &prescription); //�޸Ĵ���
	//long modifyMedicine(int prescriptionId, int medicineId, map<string,string> &medicine);		//�޸�ҩƷ
	long modifyPresMedicine(int prescriptionId, list<CPrescriptionMedicine> &preMedicine);		//�޸�ҩƷ
	long modifyPatient(string patientId, map<string,string> &patient);			//�޸Ĳ��ˣ�map�ĵ�һ��ֵΪҪ�޸ĵ��ֶΣ��ڶ���ֵΪ���ֶ�ֵ
	long deletePrescription(int prescriptionId);					//ɾ������
	long deleteMedicine(int prescriptionId, int medicineId);		//ɾ��ҩƷ
	long deletePatient(int patientId);								//ɾ��������Ϣ
	long listLastPrescriptionMedicine(list<CPrescriptionMedicine> &preMedicine, int num, string strWhere="");  //�г�����ҩƷ�嵥
    long blankOutPrescription(CPrescription &prescription, long &prescriptionId);  //���ϴ���
    string getBasketId(string cardId);
private:
	static LoggerPtr rootLogger;
	CDBManage *db;
	
	long savePatient(CPatient &prePatient, string &patientId);		//���没��
	long savePrescriptionMedicine(long prescriptionId, list<CPrescriptionMedicine> &preMedicine);  //����ҩƷ
	void getPatientFields(CPatient &prePatient);		//���ô���������Ϣ
	long getPrescriptionMedicineFields(string sqlText, list<CPrescriptionMedicine> &listPreMedicine);	//ȡ�ô���ҩƷ��Ϣ
    long savePrescription(CPrescription &prescription, long &prescriptionId);  //���洦��
};

#endif
