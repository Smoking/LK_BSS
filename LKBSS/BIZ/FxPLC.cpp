// FxPLC.cpp: implementation of the CFxPLC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FxPLC.h"
#include "ConfigUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxPLC::CFxPLC()
{
	CoInitialize(NULL);
	m_mute = new CMutex(FALSE,"CFxPLC",NULL);
	runMode = configUtil.getInt("runMode");
	connectFlag = 0;
}

CFxPLC::~CFxPLC()
{
	delete m_mute;
	CoUninitialize();
}

long CFxPLC::GetFxPLCConnectState()
{
	return connectFlag;
}

void CFxPLC::SetFxControl(CActFXCPU3 *pfxcpu)
{
	m_fxcpu = pfxcpu;
}

long CFxPLC::Open()
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	return connectFlag = m_fxcpu->Open();
}

long CFxPLC::Close()
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	return m_fxcpu->Close();
}

long CFxPLC::ReadDeviceBlock(LPCTSTR szDevice, long lSize, long* lplData)
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	m_mute->Lock();
	connectFlag = m_fxcpu->ReadDeviceBlock(szDevice,lSize, lplData);
	m_mute->Unlock();
	return connectFlag;
}

long CFxPLC::WriteDeviceBlock(LPCTSTR szDevice, long lSize, long* lplData)
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	m_mute->Lock();
	connectFlag = m_fxcpu->WriteDeviceBlock(szDevice,lSize, lplData);
	m_mute->Unlock();
	return connectFlag;
}


long CFxPLC::SetDevice(LPCTSTR szDevice, long lData)
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	m_mute->Lock();
	connectFlag = m_fxcpu->SetDevice(szDevice,lData);
	m_mute->Unlock();
	return connectFlag;
}

long CFxPLC::GetDevice(LPCTSTR szDevice, long* lplData)
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	m_mute->Lock();
	connectFlag = m_fxcpu->GetDevice(szDevice,lplData);
	m_mute->Unlock();
	return connectFlag;
}

void CFxPLC::SetActCpuType(long nNewValue)
{
	if (runMode == 1 || runMode == 9)
	{
		return ;
	}
	m_fxcpu->SetActCpuType(nNewValue);
}

void CFxPLC::SetActPortNumber(long nNewValue)
{
	if (runMode == 1 || runMode == 9)
	{
		return ;
	}
	m_fxcpu->SetActPortNumber(nNewValue);
}

long CFxPLC::GetUrgentInfo(long *pData)
{
	if (runMode == 1 || runMode == 9)
	{
		*pData = 0;
		return 0;
	}
	return connectFlag = GetDevice("M0",pData);
}

long CFxPLC::GetHaveBasketInfo(long *pData)
{
	if (runMode == 1 || runMode == 9)
	{
		*pData = 0;
		return 0;
	}
	return connectFlag = GetDevice("M1",pData);
}

long CFxPLC::GetBasketReachInfo(long *pData)
{
	if (runMode == 1 || runMode == 9)
	{
		*pData = 1;
		return 0;
	}
	return connectFlag = GetDevice("M2",pData);
}

long CFxPLC::GetAlarmInfo(long *pData)
{
	if (runMode == 1 || runMode == 9)
	{
		*pData = 0;
		return 0;
	}
	return connectFlag = GetDevice("M3",pData);
}

long CFxPLC::GetMessageAlarmInfo(long *pData)
{
	if (runMode == 1 || runMode == 9)
	{
		*pData = 0;
		return 0;
	}
	return connectFlag = GetDevice("M4",pData);
}

/*long CFxPLC::SetDistributeBasket(long Data)
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	return connectFlag = SetDevice("M5",Data);
}*/

long CFxPLC::SetIniSendBasketFinish(long Data)
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	return connectFlag = SetDevice("M11",Data);
}

long CFxPLC::SetStartInsideBelt(long Data)
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	long unitValue = 1;
	if(Data > 0)
	{	unitValue = (Data-1)/320 +2 ;
	    if(unitValue>3)
			unitValue -= 2;
	}
	return connectFlag = WriteDeviceBlock("D0",1,&unitValue);
}

long CFxPLC::SetSendBasketOut(long Data)
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	return connectFlag = SetDevice("M7",Data);
}

long CFxPLC::SetStartOutsideBelt(long Data)
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	return connectFlag = SetDevice("M8",Data);
}

long CFxPLC::SetStopOutsideBelt(long Data)
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	return connectFlag = SetDevice("M9",Data);
}

long CFxPLC::GetLackBasketInfo( long* pData )
{
	if (runMode == 1 || runMode == 9)
	{
		*pData = 0;
		return 0;
	}
	return connectFlag = m_fxcpu->GetDevice("M10",pData);
}

long CFxPLC::GetSendBasketFinishInfo(long *pData)
{
	if (runMode == 1 || runMode == 9)
	{
		*pData = 1;
		return 0;
	}
	return connectFlag = GetDevice("M11",pData);
}

long CFxPLC::GetFinishInfo(long *pData)
{
	if (runMode == 1 || runMode == 9)
	{
		*pData = 1;
		return 0;
	}
	return connectFlag = ReadDeviceBlock("D0",1,pData);
}