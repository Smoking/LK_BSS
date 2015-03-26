// FxPLC.h: interface for the CFxPLC class.
//
//////////////////////////////////////////////////////////////////////
#include "actfxcpu3.h"
#include <afxmt.h>

#if !defined(AFX_FXPLC_H__AE02F857_3A68_4877_9C27_E24EA4E4D8B7__INCLUDED_)
#define AFX_FXPLC_H__AE02F857_3A68_4877_9C27_E24EA4E4D8B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFxPLC  
{
public:
	long GetFinishInfo(long *pData);
	long GetSendBasketFinishInfo(long *pData);//������������ź�
	long GetLackBasketInfo(long* pData);//ȱ�������ź�
	long SetStopOutsideBelt(long Data);//ֹͣ�ⲿ���ʹ��ź�
	long SetStartOutsideBelt(long Data);//�����ⲿ���ʹ��ź�	
	long SetSendBasketOut(long Data);//���������ź�(�ڰ�ʧ�ܻ����ʧ��ʱ����)
	long SetStartInsideBelt(long Data);//�ڲ����ʹ������ź�
	//long SetDistributeBasket(long Data);//�����ź�
	long SetIniSendBasketFinish(long Data);//��λ������������ź�
	long GetMessageAlarmInfo(long *pData);//��Ҫͣ��ά�ޱ����ź�
	long GetAlarmInfo(long *pData);//��Ҫͣ�������ź�
	long GetBasketReachInfo(long * pData);//���ӵ����ź�
	long GetHaveBasketInfo(long * pData);//����Ƿ��������ź�
	long GetUrgentInfo(long * pData);//��ͣ�ź�
	long GetFxPLCConnectState();
	CFxPLC();
	virtual ~CFxPLC();
	CActFXCPU3	*m_fxcpu;
	void SetFxControl(CActFXCPU3 *pfxcpu);
	long Open();
	long Close();
	long ReadDeviceBlock(LPCTSTR szDevice, long lSize, long* lplData);
	long WriteDeviceBlock(LPCTSTR szDevice, long lSize, long* lplData);
	long SetDevice(LPCTSTR szDevice, long lData);
	long GetDevice(LPCTSTR szDevice, long* lplData);
	void SetActPortNumber(long nNewValue);
	void SetActCpuType(long nNewValue);

	int runMode;	//����ģʽ 
	int connectFlag;
	CMutex *m_mute;

};

#endif // !defined(AFX_FXPLC_H__AE02F857_3A68_4877_9C27_E24EA4E4D8B7__INCLUDED_)
