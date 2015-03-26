// BoxSinglechip.cpp: implementation of the CBoxSinglechip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BoxSinglechip.h"
#include "Utils.h"
#include "ConfigUtil.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ER_BOXSINGLECHIP_PORT_NUM			250001	//�м���Ƭ���˿ںŴ���
#define ER_OPEN_BOXSINGLECHIP_PORT	        250002	//�м���Ƭ���˿ڴ򿪴���
#define ER_NO_MEDINFO               	    250003	//û�з�ҩҩƷ��Ϣ
#define ER_SHOW_TEXT                        250004	//��ʾ��Ϣ��LCDʧ��
#define ER_SHOW_ROW                         250005	//��ʾ��Ϣ��LCD�в������� 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LoggerPtr CBoxSinglechip::rootLogger = Logger::getLogger("CBoxSinglechip");

CBoxSinglechip::CBoxSinglechip()
{
	runMode = configUtil.getInt("runMode");
	m_mute = new CMutex(FALSE,"CBoxSinglechip",NULL);
	initDate();
}

CBoxSinglechip::~CBoxSinglechip()
{
	delete m_mute;
}
void CBoxSinglechip::initDate()
{
	finishfalg = false;
	initiativeSendfalg = false;
    isOver = configUtil.getInt("BoxTime");;  
    dataBuf.init();
	m_lastLocationID = 0;
}


long CBoxSinglechip::open()
{
    //rootLogger->trace("function open start..............");
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
    
    int nPortNum = 0;
    int nbaudRate = 0;
    nPortNum = configUtil.getInt("SinglechipPort");
    nbaudRate = configUtil.getInt("SinglechipbaudRate");
	
    try
    {
        if (nPortNum <= 0 || nPortNum > 255)
        {
            rootLogger->error("Singlechip port error!");
            return ER_BOXSINGLECHIP_PORT_NUM;
        }
        
        if (!port.IsOpen())
        {
            port.Open(nPortNum, nbaudRate);
            port.Set0WriteTimeout();
            port.Set0ReadTimeout();   
        } 
    }catch (CSerialException* pEx)
    {
        string tmp(pEx->GetErrorMessage().GetBuffer(10));
        rootLogger->error("failed to open Singlechip port: " + tmp);
        pEx->Delete();
        return ER_OPEN_BOXSINGLECHIP_PORT;
    } 
    //rootLogger->trace("function open end..............");
    return 0;
}

void CBoxSinglechip::close()
{
	//rootLogger->trace("function close start..............");
	rootLogger->info("����close,�رմ���");
	port.Close();
	//rootLogger->trace("function close end..............");
}

long CBoxSinglechip::distributeMed(MedicineInfoList &medInfo)
{
	//rootLogger->trace("function distributeMedicine start..............");
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
    
    if(medInfo.empty())
    {
        return 0;
    }
    m_mute->Lock();
    medInfoList.clear();
    medInfoList = medInfo;
    medInfoList.sort();
    long lRet = 0;
    lRet = sendMedInfo(medInfoList);
    if (lRet)
    {
		m_mute->Unlock();
        return lRet;
    }
    lRet = startDistribute();
	m_mute->Unlock();
	//rootLogger->trace("function distributeMedicine end..............");
	return lRet;
}

long CBoxSinglechip::MergerMedInfo(MedicineInfoList &medInfo)    //�ϲ���ͬ��ַ
{
	int nCount = 0; //ҩƷ����
	long address = 0; //ҩƷ��ַ
	MedicineInfoList::iterator pMedInfo;
	for (pMedInfo = medInfo.begin(); pMedInfo != medInfo.end(); pMedInfo++)
	{
        if (pMedInfo->errNo == 5)  //����г�������
        {
            continue;
        }
		address = pMedInfo->position;   //ȡ��ҩƷλ����Ϣ
        nCount = pMedInfo->count;		//ȡ��ҩƷ��Ŀ
		MedicineInfoList::iterator iter = pMedInfo;
		iter++ ;
        while (iter != medInfo.end())
		{
			if (iter->errNo == 5)  //����г�������
			{
				iter++;
				continue;
			}
			if(iter->position == address)
			{
				pMedInfo->count +=nCount;
				medInfo.erase(iter);
			}
			else
			{
				iter++;
			}
		}	
	}
	return 0;
}

long CBoxSinglechip::sendMedInfo(MedicineInfoList &medInfo)
{
    //rootLogger->trace("function sendMedInfo start..............");
	int nCount = 0; //ҩƷ����
	long address = 0; //ҩƷ��ַ
    long lRet = 0;
    int currBoard = -1;  //��ǰ����Ĳ��
	int curUnit =-1;// ��ǰ��Ԫ��
    
    BYTE szCmd[39];
    int nSize = sizeof(szCmd);
    memset(szCmd, 0, nSize);
    szCmd[0] = 0xEA ;        //��ͷ
    szCmd[1] = 0xE5;        //��ͷ
	szCmd[4] = 0x21;        //����
	szCmd[5] = 0x02;        //����

    initDate();

	MedicineInfoList::iterator pMedInfo;
	for (pMedInfo = medInfo.begin(); pMedInfo != medInfo.end(); pMedInfo++)
	{
        if (pMedInfo->errNo == 5)  //����г�������
        {
            continue;
        }

		address = pMedInfo->position;   //ȡ��ҩƷλ����Ϣ
        nCount = pMedInfo->count;		//ȡ��ҩƷ��Ŀ

		pMedInfo->errNo = 0;

        rootLogger->debug("pMedInfo->position = " + Utils::itos(address) + "  count = " 
				+ Utils::itos(pMedInfo->count));
		if (address <= 0)
		{
			rootLogger->error("medicine position is incorrect, position = " + Utils::itos(address));
			return ER_MEDICINE_POSITION;
		}

        if (nCount < 1)
        {
            rootLogger->error("medicine count is incorrect, count = " + Utils::itos(nCount)); 
            return ER_MEDICINE_COUNT;
        }
        
        if (!port.IsOpen())
        {
            long lRet = open();
            if (lRet)
            {
				AfxMessageBox("�뵥Ƭ������ͨѶʧ�ܣ�");
                return lRet;
            }          
        }

        //ת����ź�ҩ��ID
        int boardNo = 0;
        int boxId = 0;
		int unit = (address-1)/320;//��Ԫ 0-
		address -= unit*320;
        boardNo = (address - 1)/BOXES_OF_BOARD; //0-10
        boxId = (address - 1)%BOXES_OF_BOARD;   // 0-31    

		if (unit != curUnit) //��Ԫ��һ��
		{
			if( curUnit != -1)
			{
				szCmd[nSize - 1] += szCmd[0] + szCmd[1]+ szCmd[2]+ szCmd[3]+ szCmd[4]+ szCmd[5];
				for(int ii = 0; ii < nSize; ii++)
				{
					CString str;
					str.Format("%.2x",szCmd[ii]);
					str+=" ";
					rootLogger->debug(str.GetBuffer(0));
				}
				lRet = port.Write(szCmd, nSize);  //�����Ƿ��ͳɹ��ַ���
				////rootLogger->trace("");
				if (lRet == 0)
				{
					return ER_WRITE_DATA;
				}                
				Sleep(100);
			}
			curUnit = unit;
			currBoard = boardNo;
			memset(szCmd+6, 0, nSize - 6);
			szCmd[2] = unit;        //��Ԫ
			szCmd[3] = boardNo;        //����

		}
		else
		{		
			if (boardNo != currBoard) //�㲻һ��
			{
				if (currBoard != -1)
				{
					szCmd[nSize - 1] += szCmd[0] + szCmd[1]+ szCmd[2]+ szCmd[3]+ szCmd[4]+ szCmd[5];
					for(int ii = 0; ii < nSize; ii++)
					{
						CString str;
						str.Format("%.2x",szCmd[ii]);
						str+=" ";
						rootLogger->debug(str.GetBuffer(0));
					}
					lRet = port.Write(szCmd, nSize);  //�����Ƿ��ͳɹ��ַ���
					////rootLogger->trace("");
					if (lRet == 0)
					{
						return ER_WRITE_DATA;
					}                
					Sleep(100);
				}
				currBoard = boardNo;
				memset(szCmd+6, 0, nSize - 6);
				szCmd[2] = unit;        //��Ԫ
				szCmd[3] = boardNo;        //����
			}
		}
        
        szCmd[boxId + 6] = nCount;    //��ҩ���� 
        szCmd[nSize - 1] += szCmd[boxId + 6]; //У����
	}
    
    szCmd[nSize - 1] += szCmd[0] + szCmd[1]+ szCmd[2]+ szCmd[3]+ szCmd[4]+ szCmd[5];
    lRet = port.Write(szCmd, nSize); //�������һ������� 
	for(int ii = 0; ii < nSize; ii++)
	{
		CString str;
		str.Format("%.2x",szCmd[ii]);
		str+=" ";
		rootLogger->debug(str.GetBuffer(0));
	}
    
    if (lRet == 0)
    {
        return ER_WRITE_DATA;
    }
    Sleep(100);

    //rootLogger->trace("function sendMedInfo end..............");
    return 0;
}

long CBoxSinglechip::startDistribute()
{
    //rootLogger->trace("function startDistribute start..............");
    BYTE szCmd[7];
    int nSize = sizeof(szCmd);
    memset(szCmd, 0, nSize);
    szCmd[0] = 0xEA ;        //��ͷ
    szCmd[1] = 0xE5;        //��ͷ
	szCmd[4] = 0x01;        //����
	szCmd[5] = 0x01;        //����
	szCmd[6] = szCmd[0] + szCmd[1]+ szCmd[2]+ szCmd[3]+ szCmd[4]+ szCmd[5];
    long lRet = 0;
    if (!port.IsOpen())
    {			
        long lRet = open();
        if (lRet)
        {
			AfxMessageBox("�뵥Ƭ������ͨѶʧ�ܣ�");
            return lRet;
        } 
        rootLogger->info("port reOpen");
    }
    lRet = port.Write(szCmd, nSize);
    
    if (lRet == 0)
    {
        return ER_WRITE_DATA;
    }
    Sleep(100);	
    //rootLogger->trace("function startDistribute end..............");
    return 0;
}

long CBoxSinglechip::setDistributeMedicineFlag(int flag)
{
	if(flag ==0)
	{
		medInfoList.clear();
	}
	return 0;
}

bool CBoxSinglechip::checkFinish()
{
	if(finishfalg)
	{
		return true;
	}
	MedicineInfoList::iterator pMedInfo;
	for (pMedInfo = medInfoList.begin(); pMedInfo != medInfoList.end(); pMedInfo++)
	{
        if (pMedInfo->errNo == 0) 
        {
            return false;
        }
	}
	return true;
}

long CBoxSinglechip::isDstrOver(MedicineInfoList &errorAddress)
{
    //rootLogger->trace("function isDstrOver start..............");
    long lRes = 0;
    try
    {
		if (runMode == 1 || runMode == 9)
		{
			return 0;
		}
        if (medInfoList.empty())
        {
			lRes = 0;
            return lRes;
        }

        if (isOver == 0)//�Ѿ����͹�������ʾ����ҩ״̬����ȷ��Ϊ�����򷵻س�ʱ
        {
            errorAddress = medInfoList;
            rootLogger->debug("�Ѿ����͹�������ʾ����ҩ״̬����ȷ�����򷵻س�ʱ");
            lRes = 4;
			return lRes;
        }
        
        if( checkFinish() && !medInfoList.empty())    //��ҩ����
        {
            errorAddress = medInfoList;
			rootLogger->debug("��ҩ�����ȫ�����أ����ǳ���, retunr 3");
            lRes = 3;
			return lRes;
        }

		if (!port.IsOpen())
        {			
            long lRet = open();
            if (lRet)
            {
				AfxMessageBox("�뵥Ƭ������ͨѶʧ�ܣ�");
                lRes = 5;
				return lRes;
            } 
			rootLogger->info("port reOpen");
        }	

        isOver--;
        rootLogger->debug("isOver=" + Utils::itos(isOver));
		m_mute->Lock();
        if (isOver%60 == 0 )
        {
            InitiativeSendCmd();         
        }        
		int nRet = getReturn(); 
		m_mute->Unlock();
        return nRet;
        
    }catch (CSerialException* pEx)
    {
        string tmp(pEx->GetErrorMessage().GetBuffer(10));
        rootLogger->error("failed to read data: " + tmp);
        pEx->Delete();
        lRes = ER_READ_DATA;
		rootLogger->info("try�쳣");
        return lRes;
    }

    //rootLogger->trace("function isDstrOver end..............");    
}

int CBoxSinglechip::getReturn()
{
    //rootLogger->trace("function getReturn start..............");
    int count = 0;
    count = port.BytesWaiting(); //���ڽ����ֽ���
    if (count == 0)				//û���ֽ�ֱ�ӷ���
    {
        rootLogger->debug("count = 0");
        //Sleep(50);
        return 1; 
    }

    while (port.BytesWaiting() > 0 && dataBuf.len < 1024)
    {
		count = port.BytesWaiting();
		rootLogger->debug("���շ�ҩ��������ܵ�����ֵ count = " + Utils::itos(count));
        int nTmp = 0;
        nTmp = port.Read(dataBuf.buf + dataBuf.len, count);
        dataBuf.len += nTmp;             
        Sleep(100);
    }   

	rootLogger->debug("dataBuf.len = " + Utils::itos(dataBuf.len));
    rootLogger->debug("dataBuf.currPos = " + Utils::itos(dataBuf.currPos));
	rootLogger->debug("���շ�ҩ������ص�����Ϊ:");
	for(int ii = dataBuf.currPos; ii < dataBuf.len; ii++)
	{
		CString str;
		str.Format("%.2x",dataBuf.buf[ii]);
		str+=" ";
		rootLogger->debug(str.GetBuffer(0));
	}
    
    BYTE *pChr = dataBuf.buf + dataBuf.currPos; //��ǰָ�� 
	int len = dataBuf.len - dataBuf.currPos;
	int res = analyseData(pChr,len,true);
	if(getErrorMessage())
	{
		res = 2;
	}
    //rootLogger->trace("function getReturn end..............");
    return res;
}

long CBoxSinglechip::analyseData(BYTE * pChr,int len,bool flag)
{
	BYTE cHead[5] = {0xCA ,0xC5 , 0x21 ,0x01 ,0x02};   //0x21�ǳ��� 33ֻ���� ��������ݣ� ʵ����39 
	int i = 0;
	while(i < len)
    {
        if ((*pChr == cHead[0]) && (*(pChr+1) == cHead[1]) )
        {
			if( *(pChr+4) == cHead[2] )
			{
				if (len < cHead[2]+6)
				{
					rootLogger->debug("�յ������ַ������Ȳ�����");
					return 1;
				}
				//У���
				if (!sumCheck(pChr,39))
				{
					pChr += 2;
					if(flag)
					    dataBuf.currPos += 2;
					i +=2;
					continue;
				}
				checkDstr(pChr);
				pChr += cHead[2]+6; 
				if(flag)
					dataBuf.currPos+=cHead[2]+6; 
				i+=cHead[2]+6; 
			}
			else if( *(pChr+4) == cHead[3])
			{
				if (len < cHead[3]+6)
				{
					rootLogger->debug("�յ������ַ������Ȳ�����");
					return 1;
				}
				//У���
				if (!sumCheck(pChr,7))
				{
					pChr += 2;
					if(flag)
						dataBuf.currPos += 2;
					i +=2;
					continue;
				}
				if (pChr[5] == 0x87) //�ظ���ҩ��������
				{
					finishfalg = true;
				}
				else if (pChr[5] == 0x82 || pChr[5] == 0x89) //�ظ���ҩ����
				{
					int location = pChr[2] * 10 + pChr[3] ;
					map<int ,list<int> >::iterator iter;
					iter = addMed_map.find(location) ;
					if(iter != addMed_map.end())
					{
						iter->second.push_back(1);
					}
					else
					{
						list<int> temp;
						temp.push_back(1);
						addMed_map[location]= temp ;
					}
				}
				else if (pChr[5] == 0x84 || pChr[5] == 0x8A) //�ظ�������ҩ����
				{
					int location = pChr[2] * 10 + pChr[3] ;
					map<int ,list<int> >::iterator iter;
					iter = finishMed_map.find(location) ;
					if(iter != finishMed_map.end())
					{
						iter->second.push_back(1);
					}
					else
					{
						list<int> temp;
						temp.push_back(1);
						finishMed_map[location]= temp ;
					}
				}
				else if( *(pChr+5) == 0XA1 )//��ҩָʾ �������ݰ��д�
				{
					int location = pChr[2] * 10 + pChr[3] ;

					addMedError_map[location] = 0xA1;
					
					CString str;
					str.Format("��ҩָʾ �������ݰ��д��� 0x%.2x ��Ԫ, �� 0x%.2x ��",pChr[2]+1, pChr[3]+1);
					rootLogger->debug(str.GetBuffer(0));
					str.ReleaseBuffer();
					//return 0XA1;
				}
				else if(pChr[5] == 0xA2)//��ҩָʾ �������ݰ��д�
				{
                    int location = pChr[2] * 10 + pChr[3] ;

					sendMedError_map[location] = 0xA2;
					
					CString str;
					str.Format("��ҩָʾ �������ݰ��д��� 0x%.2x ��Ԫ, �� 0x%.2x ��",pChr[2]+1, pChr[3]+1);
					rootLogger->debug(str.GetBuffer(0));
					str.ReleaseBuffer();
					//return 0xA2;
				}
				else if( *(pChr+5) == 0XA3 )//��ҩ����ư壨���豸��ͨ�����ӹ���
				{
					int location = pChr[2] * 10 + pChr[3] ;
							
					addLinkError_map[location]= 0XA3 ;
		
					CString str;
					str.Format("��ҩ����ư壨���豸��ͨ�����ӹ��ϣ��� 0x%.2x ��Ԫ, �� 0x%.2x ��",pChr[2]+1, pChr[3]+1);
					rootLogger->debug(str.GetBuffer(0));
					str.ReleaseBuffer();
					//return 0XA3;
				}				
				else if(pChr[5] == 0xA4)//��ҩ����ư壨���豸��ͨ�����ӹ���
				{
					int location = pChr[2] * 10 + pChr[3] ;
							
					sendLinkError_map[location]= 0XA4 ;
					CString str;
					str.Format("��ҩ����ư壨���豸��ͨ�����ӹ��ϣ��� 0x%.2x ��Ԫ, �� 0x%.2x ��",pChr[2]+1, pChr[3]+1);
					rootLogger->debug(str.GetBuffer(0));
					str.ReleaseBuffer();
					//return 0xA4;
				}
				else if(pChr[5] == 0xA5)//ͨ��ת�Ӱ� ���ӹ���
				{
					int location = pChr[2]  ;
							
					linkError_map[location]= 0XA5 ;
					CString str;
					str.Format("ͨ��ת�Ӱ� ���ӹ��ϣ��� 0x%.2x ת�Ӱ�",pChr[2]+1);
					rootLogger->debug(str.GetBuffer(0));
					str.ReleaseBuffer();
					//return 0xA5;
				}
				pChr += cHead[3]+6; 
				if(flag)
					dataBuf.currPos+=cHead[3]+6; 
				i+=cHead[3]+6; 				
			}
			else if(*(pChr+4) == cHead[4])
			{
				if (len < cHead[4]+6)
				{
					rootLogger->debug("�յ������ַ������Ȳ�����");
					return 1;
				}
				//У���
				if (!sumCheck(pChr,8))
				{
					pChr += 2;
					if(flag)
						dataBuf.currPos += 2;
					i +=2;
					continue;
				}
				if (pChr[5] == 0x88) //��ҩ
				{
					int location = pChr[2] * 320 + pChr[3] * 32 +pChr[6];
							
					locationError_map[location]= 0x88 ;
					CString str;
					str.Format("��ҩ���� 0x%.2x ��Ԫ, �� 0x%.2x �� ",pChr[2]+1, pChr[3]+1);
					rootLogger->debug(str.GetBuffer(0));
					str.ReleaseBuffer();
					//return 0x88;
				}
				pChr += cHead[4]+6; 
				if(flag)
					dataBuf.currPos+=cHead[4]+6; 
				i+=cHead[4]+6; 	
			}
			else
			{
				pChr += 2; 
				if(flag)
					dataBuf.currPos+=2; 
				i += 2;
			}
        }
		else
		{
			pChr += 1; 
			if(flag)
				dataBuf.currPos+=1; 
			i++;
		}
    }
	return 1;
}

bool CBoxSinglechip::getErrorMessage()
{
	bool errorflag = false ;
	map<int ,int>::iterator iter;
	for(iter = linkError_map.begin();iter != linkError_map.end();iter++)
	{
		::PostMessage(hwnd,WM_GETERRORMESSAGE,iter->second,iter->first) ;
		 errorflag = true ;
	}
	linkError_map.clear();
	for(iter = locationError_map.begin();iter != locationError_map.end();iter++)
	{
		::PostMessage(hwnd,WM_GETERRORMESSAGE,iter->second,iter->first) ;
		 errorflag = true ;
	}
    locationError_map.clear();
	for(iter = addLinkError_map.begin();iter != addLinkError_map.end();iter++)
	{
		::PostMessage(hwnd,WM_GETERRORMESSAGE,iter->second,iter->first) ;
		 errorflag = true ;
	}
    addLinkError_map.clear();
	for(iter = sendLinkError_map.begin();iter != sendLinkError_map.end();iter++)
	{
		::PostMessage(hwnd,WM_GETERRORMESSAGE,iter->second,iter->first) ;
		 errorflag = true ;
	}
    sendLinkError_map.clear();
	for(iter = addMedError_map.begin();iter != addMedError_map.end();iter++)
	{
		::PostMessage(hwnd,WM_GETERRORMESSAGE,iter->second,iter->first) ;
		 errorflag = true ;
	}
    addMedError_map.clear();
	for(iter = sendMedError_map.begin();iter != sendMedError_map.end();iter++)
	{
		::PostMessage(hwnd,WM_GETERRORMESSAGE,iter->second,iter->first) ;
		 errorflag = true ;
	}
    sendMedError_map.clear();
	return errorflag;
}

//У����ȷ����true 
bool CBoxSinglechip::sumCheck(BYTE *pData ,int len)                               //��У��
{
    //rootLogger->trace("function sumCheck start..............");
    BYTE sum = 0;
    for (int i = 0; i < len-1; i++)
    {
        sum += *(pData + i);
    }
    bool bRet = false;
    if (sum == *(pData + len-1))
    {
        bRet = true;
    }
    //rootLogger->trace("function sumCheck end..............");
    return bRet;
}

int CBoxSinglechip::checkDstr(BYTE *pChr)
{
    //����,ɾ�� 
    if (pChr[5] == 0x81 || pChr[5] == 0x86 ) //
    {
        int nAdd = pChr[2]* 320 + pChr[3] * BOXES_OF_BOARD + 1;  
        int locId = 0;
        for (int i = 0; i < 32; i++)
        {
            //if (pChr[i + 6] != 0xFF)
            {
                CString str;
                str.Format("pChr[%d+6] = 0x%.2x",i, pChr[i + 6]);
                rootLogger->debug(str.GetBuffer(0));
				str.ReleaseBuffer();
                
                locId = nAdd + i;

                MedicineInfoList::iterator itr = find_if(medInfoList.begin(), medInfoList.end(), FindByPos(locId));
                if (itr != medInfoList.end())
                {                            
                    rootLogger->debug("find_if is true");
                    if (itr->errNo != 0)
                    {
                        rootLogger->debug("itr->errNo != 0����д��������"); //
                        continue;
                    }

                    BYTE medCnt = pChr[i + 6];       //ʵ�ʷ���ҩƷ���� 
                   
                    if (itr->count == (int)medCnt)  //��ҩ�ɹ�
                    {                
                        medInfoList.erase(itr);
                        rootLogger->debug("��ҩ�ɹ�ɾ����λ�ã�" + Utils::itos(locId));
                    }
					else if (itr->count > medCnt)  //ʵ�ʷ�����
                    {
						itr->count -= (int)medCnt; 
                        itr->errNo = 4;
						rootLogger->debug("ȱҩλ�ã�" + Utils::itos(locId));
                    }
					else if (itr->count < medCnt)  //ʵ�ʷ�����
                    {
						itr->count = (int)medCnt - itr->count; 
                        itr->errNo = 5;
						rootLogger->debug("�෢λ�ã�" + Utils::itos(locId));
                    }
					m_lastLocationID = locId ;
                }
            }
        } 
    }      
    return 0;
}

int CBoxSinglechip::InitiativeSendCmd()  //������������
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	long address = 0; //ҩƷ��ַ
    long lRet = 0;
    int currBoard = -1;  //��ǰ����Ĳ��
	int curUnit =-1;// ��ǰ��Ԫ��
	BYTE szCmd[7];
    int nSize = sizeof(szCmd);
    memset(szCmd, 0, nSize);
    szCmd[0] = 0xEA ;        //��ͷ
    szCmd[1] = 0xE5;        //��ͷ
	szCmd[4] = 0x01;        //����
	szCmd[5] = 0x07;        //����
    MedicineInfoList::iterator pMedInfo;
	for (pMedInfo = medInfoList.begin(); pMedInfo != medInfoList.end(); pMedInfo++)
	{
        if (pMedInfo->errNo == 5 || pMedInfo->errNo == 4)  //����г�������
        {
            continue;
        }

		address = pMedInfo->position;   //ȡ��ҩƷλ����Ϣ

        rootLogger->debug("������ѯ��ҩ��ַ�� pMedInfo->position = " + Utils::itos(address));
		if (address <= 0)
		{
			rootLogger->error("medicine position is incorrect, position = " + Utils::itos(address));
			return ER_MEDICINE_POSITION;
		}
        
        if (!port.IsOpen())
        {
            long lRet = open();
            if (lRet)
            {
				AfxMessageBox("�뵥Ƭ������ͨѶʧ�ܣ�");
                return lRet;
            }          
        }

        int boardNo = 0; //���
		int unit = (address-1)/320;//��Ԫ
		address -= unit*320;
        boardNo = (address - 1)/BOXES_OF_BOARD;  

		if (unit != curUnit) //��Ԫ��һ��
		{
			if( curUnit != -1)
			{
				szCmd[nSize - 1] = szCmd[0] + szCmd[1]+ szCmd[2]+ szCmd[3]+ szCmd[4]+ szCmd[5];
				for(int ii = 0; ii < nSize; ii++)
				{
					CString str;
					str.Format("%.2x",szCmd[ii]);
					str+=" ";
					rootLogger->debug(str.GetBuffer(0));
				}
				lRet = port.Write(szCmd, nSize);  //�����Ƿ��ͳɹ��ַ���
				if (lRet == 0)
				{
					return ER_WRITE_DATA;
				}                
				Sleep(150);
			}
			curUnit = unit;
			currBoard = boardNo;
			szCmd[2] = unit;        //��Ԫ
			szCmd[3] = boardNo;        //����

		}
		else
		{		
			if (boardNo != currBoard) //�㲻һ��
			{
				if (currBoard != -1)
				{
					szCmd[nSize - 1] = szCmd[0] + szCmd[1]+ szCmd[2]+ szCmd[3]+ szCmd[4]+ szCmd[5];
					lRet = port.Write(szCmd, nSize);  //�����Ƿ��ͳɹ��ַ���
					if (lRet == 0)
					{
						return ER_WRITE_DATA;
					}                
					Sleep(150);
				}
				currBoard = boardNo;
				szCmd[2] = unit;        //��Ԫ
				szCmd[3] = boardNo;        //����
			}
		}
	}   
    szCmd[nSize - 1] = szCmd[0] + szCmd[1]+ szCmd[2]+ szCmd[3]+ szCmd[4]+ szCmd[5];
	for(int ii = 0; ii < nSize; ii++)
	{
		CString str;
		str.Format("%.2x",szCmd[ii]);
		str+=" ";
		rootLogger->debug(str.GetBuffer(0));
	}
    lRet = port.Write(szCmd, nSize); //�������һ������� 
	return 0;
}

long CBoxSinglechip::fillMedicineStartBox(MedicineInfo &medInfo)	//�м���ҩ��ʼ
{
    //rootLogger->trace("function fillMedicineStartBox start..............");
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	m_mute->Lock();

	int address = 0; //ҩƷ��λ
	long lRet = 0;

	address = medInfo.position;
    rootLogger->debug("��ʼ��ҩ medInfo.position = " + Utils::itos(medInfo.position));
	if (address <= 0)
	{
		rootLogger->error("failed, code:" + Utils::itos(ER_MEDICINE_POSITION));
		return ER_MEDICINE_POSITION;
	}
    
	if (!port.IsOpen())
	{
		long lRet = open();
		if (lRet)
		{
			AfxMessageBox("�뵥Ƭ������ͨѶʧ�ܣ�");
			m_mute->Unlock();
			return lRet;
		}          
	}
	//ת����ź�ҩ��ID
	int boardNo = 0;  //���
	int boxId = 0;    //�ۺ�
	int unit = (address-1)/320;//��Ԫ
	address -= unit*320;
	boardNo = (address - 1)/BOXES_OF_BOARD; //0-
	boxId = (address - 1)%BOXES_OF_BOARD;   // 0-31    

	BYTE szCmd[39];
    int nSize = sizeof(szCmd);
    memset(szCmd, 0, nSize);
    szCmd[0] = 0xEA ;        //��ͷ
    szCmd[1] = 0xE5;        //��ͷ
	szCmd[2] = unit;        //��Ԫ
	szCmd[3] = boardNo;        //����
	szCmd[4] = 0x21;        //����
	if(unit > 1)
		szCmd[5] = 0x08;        //��е�ּ�ҩ����
	else
	    szCmd[5] = 0x03;        //����
	szCmd[6+boxId] =1;

	szCmd[nSize - 1] = szCmd[0] + szCmd[1]+ szCmd[2]+ szCmd[3]+ szCmd[4]+ szCmd[5] +szCmd[6+boxId];
	for(int ii = 0; ii < nSize; ii++)
	{
		CString str;
		str.Format("%.2x",szCmd[ii]);
		str+=" ";
		rootLogger->debug(str.GetBuffer(0));
	}
	lRet = port.Write(szCmd, nSize);  //�����Ƿ��ͳɹ��ַ���
	if (lRet == 0)
	{
		m_mute->Unlock();
		return ER_WRITE_DATA;
	}  
	m_mute->Unlock();
	return 0;
	Sleep(700);   
	int res ;
	int sum = 3 ;
	while( sum > 0)
	{	
		int count = 50;
		while (port.BytesWaiting() == 0 && count >0)				//û���ֽ�ֱ�ӷ���
		{
			rootLogger->debug("port.BytesWaiting() = 0");     
			count--;
			Sleep(50);
		}
		
		DataBuf  recBuf;
		recBuf.init();		
		while (port.BytesWaiting() && recBuf.len < 1024)
		{
			count = port.BytesWaiting();
			rootLogger->debug("���տ�ʼ��ҩ��������ܵ�����ֵ count = " + Utils::itos(count));
			int nTmp = 0;
			nTmp = port.Read(recBuf.buf + recBuf.len, count);
			recBuf.len += nTmp;             
			Sleep(100);
		}   
		rootLogger->debug("���տ�ʼ��ҩ������ص�����:");
		for( ii = 0; ii < recBuf.len; ii++)
		{
			CString str;
			str.Format("%.2x",recBuf.buf[ii]);
			str+=" ";
			rootLogger->debug(str.GetBuffer(0));
		}		
		BYTE *pChr = recBuf.buf + recBuf.currPos; //��ǰָ��;
		int len = recBuf.len - recBuf.currPos;
		res = analyseData(pChr,len);	
		int location = unit * 10 + boardNo ;
		map<int ,list<int> >::iterator iter;
		iter = addMed_map.find(location) ;
		if(iter != addMed_map.end())
		{
			if(iter->second.size() > 1)
			{
				iter->second.pop_front();
			}
			else if(iter->second.size() == 1)
			{
				addMed_map.erase(iter);
			}
			res = 0;
		}
		else
		{
			sum--;
		}	
		if(getErrorMessage())
		{
			res = FILLMED_ERROR;
			break ;
		}
		if(res == 0)
		{
			break;
		}
	}
	if(res==1)
	{
		return FILLMED_ERROR;
	}
	//rootLogger->trace("function fillMedicineStartBox end..............");
	return res;
}


long CBoxSinglechip::fillMedicineFinishBox(MedicineInfo &medInfo)	//�м���ҩ����
{
    //rootLogger->trace("function fillMedicineFinishBox start..............");
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
    m_mute->Lock();
	int address = 0; //ҩƷ��λ
	long lRet = 0;
	address = medInfo.position;
    rootLogger->debug("��ҩ����medInfo.position = " + Utils::itos(medInfo.position));
	if (address <= 0)
	{
		rootLogger->error("failed, code:" + Utils::itos(ER_MEDICINE_POSITION));
		m_mute->Unlock();
		return ER_MEDICINE_POSITION;
	}
    
	if (!port.IsOpen())
	{
		long lRet = open();
		if (lRet)
		{
			AfxMessageBox("�뵥Ƭ������ͨѶʧ�ܣ�");
			m_mute->Unlock();
			return lRet;
		}          
	}
	int boardNo = 0;  //���
	int boxId = 0;    //�ۺ�
	int unit = (address-1)/320;//��Ԫ
	address -= unit*320;
	boardNo = (address - 1)/BOXES_OF_BOARD; //0-
	boxId = (address - 1)%BOXES_OF_BOARD;   // 0-31  
	
    BYTE szCmd[39];
    int nSize = sizeof(szCmd);
    memset(szCmd, 0, nSize);
    szCmd[0] = 0xEA ;        //��ͷ
	szCmd[1] = 0xE5;        //��ͷ
	szCmd[2] = unit;        //��Ԫ
	szCmd[3] = boardNo;        //����  
	szCmd[4] = 0x21;        //����
	if(unit > 1)
		szCmd[5] = 0x09;        //��е�ּ�ҩ��������
	else
	    szCmd[5] = 0x04;        //����
	
	szCmd[6+boxId] = 1;

	szCmd[nSize - 1] = szCmd[0] + szCmd[1]+ szCmd[2]+ szCmd[3]+ szCmd[4]+ szCmd[5]+ szCmd[6+boxId] ;
	for(int ii = 0; ii < nSize; ii++)
	{
		CString str;
		str.Format("%.2x",szCmd[ii]);
		str+=" ";
		rootLogger->debug(str.GetBuffer(0));
	}
	lRet = port.Write(szCmd, nSize);  //�����Ƿ��ͳɹ��ַ���
	if (lRet == 0)
	{
		m_mute->Unlock();
		return ER_WRITE_DATA;
	} 
	m_mute->Unlock();
	return 0 ;
	Sleep(700);    
	int res ;
	int sum = 3 ;	
	while( sum > 0)
	{	
		int count = 50;
		while (port.BytesWaiting() == 0 && count >0)				//û���ֽ�ֱ�ӷ���
		{
			rootLogger->debug("port.BytesWaiting() = 0");     
			count--;
			Sleep(50);
		}
		
		DataBuf  recBuf;
		recBuf.init();
		while (port.BytesWaiting() && recBuf.len < 1024)
		{
			count = port.BytesWaiting();
			rootLogger->debug("���ռ�ҩ������������ܵ�����ֵ count = " + Utils::itos(count));
			int nTmp = 0;
			nTmp = port.Read(recBuf.buf + recBuf.len, count);
			recBuf.len += nTmp;             
			Sleep(100);
		} 
		rootLogger->debug("���ռ�ҩ����������ص�����:" );
		for( ii = 0; ii < recBuf.len; ii++)
		{
			CString str;
			str.Format("%.2x",recBuf.buf[ii]);
			str+=" ";
			rootLogger->debug(str.GetBuffer(0));
		}
		
		BYTE *pChr = recBuf.buf + recBuf.currPos; //��ǰָ��;
		int len = recBuf.len - recBuf.currPos;

		res = analyseData(pChr,recBuf.len);
		
		int location = unit * 10 + boardNo ;
		map<int ,list<int> >::iterator iter;
		iter = finishMed_map.find(location) ;
		if(iter != finishMed_map.end())
		{
			if(iter->second.size() > 1)
			{
				iter->second.pop_front();
			}
			else if(iter->second.size() == 1)
			{
				finishMed_map.erase(iter);
			}
			res = 0;
		}
		else
		{
			sum--;
		}	
		if(getErrorMessage())
		{
			res = FINISHFILLMED_ERROR;
			break ;
		}
		if(res == 0)
		{
			break;
		}
	}
    //rootLogger->trace("function fillMedicineFinishBox end..............");
	if(res==1)
	{
		return FINISHFILLMED_ERROR;
	}
    return res;	
}

long CBoxSinglechip::getLastLocationID()
{
	int unit=(m_lastLocationID-1)/320+1;
	m_lastLocationID-=(unit-1)*320;
	long line=(m_lastLocationID-1)/32+1;
	return line;	
}