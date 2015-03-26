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
	long GetSendBasketFinishInfo(long *pData);//送走篮子完成信号
	long GetLackBasketInfo(long* pData);//缺少篮子信号
	long SetStopOutsideBelt(long Data);//停止外部传送带信号
	long SetStartOutsideBelt(long Data);//启动外部传送带信号	
	long SetSendBasketOut(long Data);//送走篮子信号(在绑卡失败或读卡失败时启用)
	long SetStartInsideBelt(long Data);//内部传送带启动信号
	//long SetDistributeBasket(long Data);//分篮信号
	long SetIniSendBasketFinish(long Data);//复位送走篮子完成信号
	long GetMessageAlarmInfo(long *pData);//需要停机维修报警信号
	long GetAlarmInfo(long *pData);//需要停机报警信号
	long GetBasketReachInfo(long * pData);//篮子到达信号
	long GetHaveBasketInfo(long * pData);//检测是否有篮子信号
	long GetUrgentInfo(long * pData);//急停信号
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

	int runMode;	//运行模式 
	int connectFlag;
	CMutex *m_mute;

};

#endif // !defined(AFX_FXPLC_H__AE02F857_3A68_4877_9C27_E24EA4E4D8B7__INCLUDED_)
