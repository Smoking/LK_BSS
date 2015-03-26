// Statistic.h: interface for the CStatistic class.
//author:liuweiping 
//synopsis:ȫ��ͳ��ģ��
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATISTIC_H__3656DCDB_7406_437A_BD22_3CC561CB1870__INCLUDED_)
#define AFX_STATISTIC_H__3656DCDB_7406_437A_BD22_3CC561CB1870__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include "DBManage.h"
#include "Singleton.h"
#include "SysConfig.h"
#include "ErrorCodeRes.h"

using namespace std;
using namespace log4cxx;

#define Statistic_medType_1  1001   //ҩƷ����(�м�)
#define Statistic_medType_2  1002   //ҩƷ����(Ƭ��)
#define Statistic_medType_3  1003   //ҩƷ����(���)
#define Statistic_medType_9  1009   //ҩƷ����(����)
#define Statistic_Location_1  2001  //λ��ͳ������(���)
#define Statistic_Location_2  2002  //λ��ͳ������(�����)
#define Statistic_SumLayMedicinCount 1   //ͳ������(�ܰ�ҩ����)
#define Statistic_SumPrescriptionCount 2 //ͳ������(�ܴ�������)
#define Statistic_SumDeviceRunTime 3     //ͳ������(���豸����ʱ��)
#define Statistic_SumLogCount 4          //ͳ������(�ܵ�¼����)
#define Statistic_SumPrintCount 5        //ͳ������(��������ӡ����)
#define Statistic_SumSatoPrintCount 6    //ͳ������(����ת��ӡ����)
#define Statistic_SumPackCount 7         //ͳ������(�ܰ�װ����)

//ҩƷ����ͳ�����ͽṹ
typedef struct _MedicinSort{
 //   int medID;//ҩƷID
	int medType;//ҩƷ����
    int count;//ҩƷ��ҩ����
    string medName;//ҩƷ��
	string unit;//ҩƷ��λ
} MedicinSort;

//λ��ͳ�����ͽṹ
typedef struct _LocationStatistic{
 //   int locationID;//λ��ID
	int locationType;//λ��ͳ������
    int count;//��ǰͳ�ƴ���
    int sumCount;//��ͳ�ƴ���
    string name;//ͳ������(�������)
} LocationStatistic;


class CStatistic  
{
public:
	int getBooking(map<string,int>*outBooking);//���Ԥ����
	int setBooking(map<string,string>&inBooking);//����Ԥ����
	int initialize ();//��ʼ��ȫ��ͳ������
	int saveStatistic();//����ȫ��ͳ������
	int getLocationStatistic(map<int,LocationStatistic>*plocationStatistic);//���λ��ͳ��
	int addLocationStatistic(map<int,LocationStatistic>&locationStatistic);//����λ��ͳ��
	int getMedicinSortStatistic(map<int,MedicinSort>*pmedicinSort);//���ҩƷ����ͳ��
	int addMedicinSortStatistic(map<int,MedicinSort>&medicinSort);//����ҩƷ����ͳ��
	int getSumPackCount();//�����ܰ�װ����
	int addSumPackCount(int i);//�����ܰ�װ����
	int getSumSatoPrintCount();//��������ת��ӡ����
	int addSumSatoPrintCount(int i);//��������ת��ӡ����
	int getSumPrintCount();//������������ӡ����
	int addSumPrintCount(int i);//������������ӡ����
	int getSumLogCount();//�����ܵ�¼����
	int addSumLogCount(int i);//�����ܵ�¼����
	string getSumDeviceRunTime();//������豸����ʱ��
	int startDeviceRunTime();//��¼�����豸���еĿ�ʼʱ��
	int getSumPrescriptionCount ();//�����ܴ�������
	int addSumPrescriptionCount (int i);//�����ܴ�������
	int getSumLayMedicinCount();//�����ܰ�ҩ����
	int addSumLayMedicinCount (int i);//�����ܰ�ҩ����
	CStatistic();
	virtual ~CStatistic();
private:
    static int SumLayMedicinCount;  //�ܰ�ҩ����
    static int SumPrescriptionCount;  //�ܴ�������
    static DWORD DeviceRunTime;   //�����豸���п�ʼʱ��
    static DWORD SumDeviceRunTime;   //���豸����ʱ��(����)
    static int SumLogCount;        //�ܵ�¼����
    static int SumPrintCount;       //��������ӡ����
    static int SumSatoPrintCount;   //����ת��ӡ����
    static int SumPackCount;      //�ܰ�װ����
    static map<int,MedicinSort>medicinsort;    //ҩƷ����ͳ�� (�ؼ���:ҩƷID)
    static map<int,LocationStatistic>locationstatistic;    //λ��ͳ��(�ؼ���:λ��ID)
//	static map<string,int>booking;  //Ԥ����
	static LoggerPtr rootLogger ;
	string machineID;
	CDBManage  *db;
	CSysConfig* cSysConfig;
	Singleton<CSysConfig> singleton;
};


#endif // !defined(AFX_STATISTIC_H__3656DCDB_7406_437A_BD22_3CC561CB1870__INCLUDED_)
