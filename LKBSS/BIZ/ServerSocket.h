// ServerSocket.h: interface for the CServerSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSOCKET_H__0075F461_91B0_4588_923A_0F460A87587E__INCLUDED_)
#define AFX_SERVERSOCKET_H__0075F461_91B0_4588_923A_0F460A87587E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <winsock2.h>
#include "BIZBaseService.h"
#include "BIZBoxService.h"
#include "Singleton.h"
#include "BIZFillMedicine.h"
#include "MedicineManage.h"
#include "DBManage.h"

//ҩ����Ϣ�ṹ
typedef struct _PrescriptionMed1{
 
	///ҩƷ���
	char medicineId[20];//�����ṩ

	//ҩƷ����
	char medicineNum[5];//�����ṩ

	//����������(����Ƭ������)
	char medicineHeteromorphism[5];

	//ҩƷ�÷�����ע
	char medicineHint[50];

	//��ҩ����
	char useDays[5];//�����ṩ

	//ÿ����ҩ����
	char useTime[5];//�����ṩ

	//ÿ������
	char useQty[5];//�����ṩ


}PrescriptionMed1;

//������Ϣ�ṹ
typedef struct _Prescription1{

	//HIS�������
	char prescriptionHISId[20];

	//��������
	char prescriptionType[5];//0����
	
	//ҽ�����
	char prescriptionDoctorId[20];//int 0
	
	//ҽ������
	char prescriptionDoctorName[20];
	
	//������ע
	char prescriptionHint[50];
	
	//���ұ��
	char departmentId[20];//int 0
	
	//��������
	char departmentName[30];
	
	//����
	char fuQty[5];//int 0

	//ȡҩ����
	char fetchWindow[10];//int 0

}Prescription1;

//������Ϣ�ṹ
typedef struct _PatientInfo1{

	//���ƺ�
	char mzNo[20]; //Ψһ

	//סԺ���
	char inId[20];//int 0

	//����
	char bedNo[10];//int 0
	
	//��������
	char patientName[20];

	//����Ӣ����
	char patientNameEn[20];
	
	//��������
	char patientType[5];//int 0
	
	//���߱�ע
	char patientHint[50];
	
	//��������
	char patientAge[5];//int 0
	
	//�����Ԅe
	char patientSex[5];//1Ϊ�У�2ΪŮ
	
}PatientInfo1;

//���������ṹ
typedef struct _PreOperate{
	int preId;//������
	int Operatefalg;//������
}PreOperate;

//�������ṹ
typedef struct _ReadCard{
	int basketId;//���Ӻ�
	int windowId;//���ں�
}ReadCard;

typedef struct _AddMedinceInfo{
	int medPosition;	//ҩƷλ��
	int medNum;			//ҩƷ����
	int oprType;		//���� 0-��ҩ��ʼ��1-��ҩ���
}AddMedinceInfo;


class CServerSocket  
{
public:
	CServerSocket();
	virtual ~CServerSocket();
	SOCKET m_Listening;
	HANDLE m_hListenThread,m_hAcceptThread;
	static DWORD WINAPI ListenThread(LPVOID lpparam);
	static DWORD WINAPI AcceptThread(LPVOID lpparam);
	int start();
    static bool falg;
	static CString filepath;
	static LoggerPtr rootLogger;
};

#endif // !defined(AFX_SERVERSOCKET_H__0075F461_91B0_4588_923A_0F460A87587E__INCLUDED_)
