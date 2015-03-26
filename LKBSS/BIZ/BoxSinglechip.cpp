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

#define ER_BOXSINGLECHIP_PORT_NUM			250001	//盒剂单片机端口号错误
#define ER_OPEN_BOXSINGLECHIP_PORT	        250002	//盒剂单片机端口打开错误
#define ER_NO_MEDINFO               	    250003	//没有发药药品信息
#define ER_SHOW_TEXT                        250004	//显示信息到LCD失败
#define ER_SHOW_ROW                         250005	//显示信息到LCD行参数错误 

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
	rootLogger->info("调用close,关闭串口");
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

long CBoxSinglechip::MergerMedInfo(MedicineInfoList &medInfo)    //合并相同地址
{
	int nCount = 0; //药品数量
	long address = 0; //药品地址
	MedicineInfoList::iterator pMedInfo;
	for (pMedInfo = medInfo.begin(); pMedInfo != medInfo.end(); pMedInfo++)
	{
        if (pMedInfo->errNo == 5)  //如果有出错，跳过
        {
            continue;
        }
		address = pMedInfo->position;   //取出药品位置信息
        nCount = pMedInfo->count;		//取出药品数目
		MedicineInfoList::iterator iter = pMedInfo;
		iter++ ;
        while (iter != medInfo.end())
		{
			if (iter->errNo == 5)  //如果有出错，跳过
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
	int nCount = 0; //药品数量
	long address = 0; //药品地址
    long lRet = 0;
    int currBoard = -1;  //当前处理的层号
	int curUnit =-1;// 当前单元号
    
    BYTE szCmd[39];
    int nSize = sizeof(szCmd);
    memset(szCmd, 0, nSize);
    szCmd[0] = 0xEA ;        //贞头
    szCmd[1] = 0xE5;        //贞头
	szCmd[4] = 0x21;        //长度
	szCmd[5] = 0x02;        //命令

    initDate();

	MedicineInfoList::iterator pMedInfo;
	for (pMedInfo = medInfo.begin(); pMedInfo != medInfo.end(); pMedInfo++)
	{
        if (pMedInfo->errNo == 5)  //如果有出错，跳过
        {
            continue;
        }

		address = pMedInfo->position;   //取出药品位置信息
        nCount = pMedInfo->count;		//取出药品数目

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
				AfxMessageBox("与单片机串口通讯失败！");
                return lRet;
            }          
        }

        //转换板号和药盒ID
        int boardNo = 0;
        int boxId = 0;
		int unit = (address-1)/320;//单元 0-
		address -= unit*320;
        boardNo = (address - 1)/BOXES_OF_BOARD; //0-10
        boxId = (address - 1)%BOXES_OF_BOARD;   // 0-31    

		if (unit != curUnit) //单元不一样
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
				lRet = port.Write(szCmd, nSize);  //返回是发送成功字符数
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
			szCmd[2] = unit;        //单元
			szCmd[3] = boardNo;        //层数

		}
		else
		{		
			if (boardNo != currBoard) //层不一样
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
					lRet = port.Write(szCmd, nSize);  //返回是发送成功字符数
					////rootLogger->trace("");
					if (lRet == 0)
					{
						return ER_WRITE_DATA;
					}                
					Sleep(100);
				}
				currBoard = boardNo;
				memset(szCmd+6, 0, nSize - 6);
				szCmd[2] = unit;        //单元
				szCmd[3] = boardNo;        //层数
			}
		}
        
        szCmd[boxId + 6] = nCount;    //发药数量 
        szCmd[nSize - 1] += szCmd[boxId + 6]; //校验码
	}
    
    szCmd[nSize - 1] += szCmd[0] + szCmd[1]+ szCmd[2]+ szCmd[3]+ szCmd[4]+ szCmd[5];
    lRet = port.Write(szCmd, nSize); //发送最后一块板数据 
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
    szCmd[0] = 0xEA ;        //贞头
    szCmd[1] = 0xE5;        //贞头
	szCmd[4] = 0x01;        //长度
	szCmd[5] = 0x01;        //命令
	szCmd[6] = szCmd[0] + szCmd[1]+ szCmd[2]+ szCmd[3]+ szCmd[4]+ szCmd[5];
    long lRet = 0;
    if (!port.IsOpen())
    {			
        long lRet = open();
        if (lRet)
        {
			AfxMessageBox("与单片机串口通讯失败！");
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

        if (isOver == 0)//已经发送过主动请示，发药状态还不确定为出错，则返回超时
        {
            errorAddress = medInfoList;
            rootLogger->debug("已经发送过主动请示，发药状态还不确定，则返回超时");
            lRes = 4;
			return lRes;
        }
        
        if( checkFinish() && !medInfoList.empty())    //发药出错
        {
            errorAddress = medInfoList;
			rootLogger->debug("发药结果已全部返回，但是出错, retunr 3");
            lRes = 3;
			return lRes;
        }

		if (!port.IsOpen())
        {			
            long lRet = open();
            if (lRet)
            {
				AfxMessageBox("与单片机串口通讯失败！");
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
		rootLogger->info("try异常");
        return lRes;
    }

    //rootLogger->trace("function isDstrOver end..............");    
}

int CBoxSinglechip::getReturn()
{
    //rootLogger->trace("function getReturn start..............");
    int count = 0;
    count = port.BytesWaiting(); //串口接收字节数
    if (count == 0)				//没有字节直接返回
    {
        rootLogger->debug("count = 0");
        //Sleep(50);
        return 1; 
    }

    while (port.BytesWaiting() > 0 && dataBuf.len < 1024)
    {
		count = port.BytesWaiting();
		rootLogger->debug("接收发药结果返回总的数据值 count = " + Utils::itos(count));
        int nTmp = 0;
        nTmp = port.Read(dataBuf.buf + dataBuf.len, count);
        dataBuf.len += nTmp;             
        Sleep(100);
    }   

	rootLogger->debug("dataBuf.len = " + Utils::itos(dataBuf.len));
    rootLogger->debug("dataBuf.currPos = " + Utils::itos(dataBuf.currPos));
	rootLogger->debug("接收发药结果返回的数据为:");
	for(int ii = dataBuf.currPos; ii < dataBuf.len; ii++)
	{
		CString str;
		str.Format("%.2x",dataBuf.buf[ii]);
		str+=" ";
		rootLogger->debug(str.GetBuffer(0));
	}
    
    BYTE *pChr = dataBuf.buf + dataBuf.currPos; //当前指针 
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
	BYTE cHead[5] = {0xCA ,0xC5 , 0x21 ,0x01 ,0x02};   //0x21是长度 33只包含 命令和数据， 实际是39 
	int i = 0;
	while(i < len)
    {
        if ((*pChr == cHead[0]) && (*(pChr+1) == cHead[1]) )
        {
			if( *(pChr+4) == cHead[2] )
			{
				if (len < cHead[2]+6)
				{
					rootLogger->debug("收到返回字符串长度不够！");
					return 1;
				}
				//校验和
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
					rootLogger->debug("收到返回字符串长度不够！");
					return 1;
				}
				//校验和
				if (!sumCheck(pChr,7))
				{
					pChr += 2;
					if(flag)
						dataBuf.currPos += 2;
					i +=2;
					continue;
				}
				if (pChr[5] == 0x87) //回复发药结束命令
				{
					finishfalg = true;
				}
				else if (pChr[5] == 0x82 || pChr[5] == 0x89) //回复加药命令
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
				else if (pChr[5] == 0x84 || pChr[5] == 0x8A) //回复结束加药命令
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
				else if( *(pChr+5) == 0XA1 )//补药指示 接收数据包有错
				{
					int location = pChr[2] * 10 + pChr[3] ;

					addMedError_map[location] = 0xA1;
					
					CString str;
					str.Format("补药指示 接收数据包有错：第 0x%.2x 单元, 第 0x%.2x 层",pChr[2]+1, pChr[3]+1);
					rootLogger->debug(str.GetBuffer(0));
					str.ReleaseBuffer();
					//return 0XA1;
				}
				else if(pChr[5] == 0xA2)//发药指示 接收数据包有错
				{
                    int location = pChr[2] * 10 + pChr[3] ;

					sendMedError_map[location] = 0xA2;
					
					CString str;
					str.Format("发药指示 接收数据包有错：第 0x%.2x 单元, 第 0x%.2x 层",pChr[2]+1, pChr[3]+1);
					rootLogger->debug(str.GetBuffer(0));
					str.ReleaseBuffer();
					//return 0xA2;
				}
				else if( *(pChr+5) == 0XA3 )//补药层控制板（从设备）通信连接故障
				{
					int location = pChr[2] * 10 + pChr[3] ;
							
					addLinkError_map[location]= 0XA3 ;
		
					CString str;
					str.Format("补药层控制板（从设备）通信连接故障：第 0x%.2x 单元, 第 0x%.2x 层",pChr[2]+1, pChr[3]+1);
					rootLogger->debug(str.GetBuffer(0));
					str.ReleaseBuffer();
					//return 0XA3;
				}				
				else if(pChr[5] == 0xA4)//发药层控制板（从设备）通信连接故障
				{
					int location = pChr[2] * 10 + pChr[3] ;
							
					sendLinkError_map[location]= 0XA4 ;
					CString str;
					str.Format("发药层控制板（从设备）通信连接故障：第 0x%.2x 单元, 第 0x%.2x 层",pChr[2]+1, pChr[3]+1);
					rootLogger->debug(str.GetBuffer(0));
					str.ReleaseBuffer();
					//return 0xA4;
				}
				else if(pChr[5] == 0xA5)//通信转接板 连接故障
				{
					int location = pChr[2]  ;
							
					linkError_map[location]= 0XA5 ;
					CString str;
					str.Format("通信转接板 连接故障：第 0x%.2x 转接板",pChr[2]+1);
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
					rootLogger->debug("收到返回字符串长度不够！");
					return 1;
				}
				//校验和
				if (!sumCheck(pChr,8))
				{
					pChr += 2;
					if(flag)
						dataBuf.currPos += 2;
					i +=2;
					continue;
				}
				if (pChr[5] == 0x88) //卡药
				{
					int location = pChr[2] * 320 + pChr[3] * 32 +pChr[6];
							
					locationError_map[location]= 0x88 ;
					CString str;
					str.Format("卡药：第 0x%.2x 单元, 第 0x%.2x 层 ",pChr[2]+1, pChr[3]+1);
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

//校验正确返回true 
bool CBoxSinglechip::sumCheck(BYTE *pData ,int len)                               //和校验
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
    //换算,删除 
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
                        rootLogger->debug("itr->errNo != 0，已写过错误码"); //
                        continue;
                    }

                    BYTE medCnt = pChr[i + 6];       //实际发的药品数量 
                   
                    if (itr->count == (int)medCnt)  //发药成功
                    {                
                        medInfoList.erase(itr);
                        rootLogger->debug("发药成功删除，位置：" + Utils::itos(locId));
                    }
					else if (itr->count > medCnt)  //实际发少了
                    {
						itr->count -= (int)medCnt; 
                        itr->errNo = 4;
						rootLogger->debug("缺药位置：" + Utils::itos(locId));
                    }
					else if (itr->count < medCnt)  //实际发多了
                    {
						itr->count = (int)medCnt - itr->count; 
                        itr->errNo = 5;
						rootLogger->debug("多发位置：" + Utils::itos(locId));
                    }
					m_lastLocationID = locId ;
                }
            }
        } 
    }      
    return 0;
}

int CBoxSinglechip::InitiativeSendCmd()  //主动发送命令
{
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	long address = 0; //药品地址
    long lRet = 0;
    int currBoard = -1;  //当前处理的层号
	int curUnit =-1;// 当前单元号
	BYTE szCmd[7];
    int nSize = sizeof(szCmd);
    memset(szCmd, 0, nSize);
    szCmd[0] = 0xEA ;        //贞头
    szCmd[1] = 0xE5;        //贞头
	szCmd[4] = 0x01;        //长度
	szCmd[5] = 0x07;        //命令
    MedicineInfoList::iterator pMedInfo;
	for (pMedInfo = medInfoList.begin(); pMedInfo != medInfoList.end(); pMedInfo++)
	{
        if (pMedInfo->errNo == 5 || pMedInfo->errNo == 4)  //如果有出错，跳过
        {
            continue;
        }

		address = pMedInfo->position;   //取出药品位置信息

        rootLogger->debug("主动查询发药地址： pMedInfo->position = " + Utils::itos(address));
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
				AfxMessageBox("与单片机串口通讯失败！");
                return lRet;
            }          
        }

        int boardNo = 0; //层号
		int unit = (address-1)/320;//单元
		address -= unit*320;
        boardNo = (address - 1)/BOXES_OF_BOARD;  

		if (unit != curUnit) //单元不一样
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
				lRet = port.Write(szCmd, nSize);  //返回是发送成功字符数
				if (lRet == 0)
				{
					return ER_WRITE_DATA;
				}                
				Sleep(150);
			}
			curUnit = unit;
			currBoard = boardNo;
			szCmd[2] = unit;        //单元
			szCmd[3] = boardNo;        //层数

		}
		else
		{		
			if (boardNo != currBoard) //层不一样
			{
				if (currBoard != -1)
				{
					szCmd[nSize - 1] = szCmd[0] + szCmd[1]+ szCmd[2]+ szCmd[3]+ szCmd[4]+ szCmd[5];
					lRet = port.Write(szCmd, nSize);  //返回是发送成功字符数
					if (lRet == 0)
					{
						return ER_WRITE_DATA;
					}                
					Sleep(150);
				}
				currBoard = boardNo;
				szCmd[2] = unit;        //单元
				szCmd[3] = boardNo;        //层数
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
    lRet = port.Write(szCmd, nSize); //发送最后一块板数据 
	return 0;
}

long CBoxSinglechip::fillMedicineStartBox(MedicineInfo &medInfo)	//盒剂加药开始
{
    //rootLogger->trace("function fillMedicineStartBox start..............");
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
	m_mute->Lock();

	int address = 0; //药品槽位
	long lRet = 0;

	address = medInfo.position;
    rootLogger->debug("开始加药 medInfo.position = " + Utils::itos(medInfo.position));
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
			AfxMessageBox("与单片机串口通讯失败！");
			m_mute->Unlock();
			return lRet;
		}          
	}
	//转换板号和药盒ID
	int boardNo = 0;  //层号
	int boxId = 0;    //槽号
	int unit = (address-1)/320;//单元
	address -= unit*320;
	boardNo = (address - 1)/BOXES_OF_BOARD; //0-
	boxId = (address - 1)%BOXES_OF_BOARD;   // 0-31    

	BYTE szCmd[39];
    int nSize = sizeof(szCmd);
    memset(szCmd, 0, nSize);
    szCmd[0] = 0xEA ;        //贞头
    szCmd[1] = 0xE5;        //贞头
	szCmd[2] = unit;        //单元
	szCmd[3] = boardNo;        //层数
	szCmd[4] = 0x21;        //长度
	if(unit > 1)
		szCmd[5] = 0x08;        //机械手加药命令
	else
	    szCmd[5] = 0x03;        //命令
	szCmd[6+boxId] =1;

	szCmd[nSize - 1] = szCmd[0] + szCmd[1]+ szCmd[2]+ szCmd[3]+ szCmd[4]+ szCmd[5] +szCmd[6+boxId];
	for(int ii = 0; ii < nSize; ii++)
	{
		CString str;
		str.Format("%.2x",szCmd[ii]);
		str+=" ";
		rootLogger->debug(str.GetBuffer(0));
	}
	lRet = port.Write(szCmd, nSize);  //返回是发送成功字符数
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
		while (port.BytesWaiting() == 0 && count >0)				//没有字节直接返回
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
			rootLogger->debug("接收开始加药结果返回总的数据值 count = " + Utils::itos(count));
			int nTmp = 0;
			nTmp = port.Read(recBuf.buf + recBuf.len, count);
			recBuf.len += nTmp;             
			Sleep(100);
		}   
		rootLogger->debug("接收开始加药结果返回的数据:");
		for( ii = 0; ii < recBuf.len; ii++)
		{
			CString str;
			str.Format("%.2x",recBuf.buf[ii]);
			str+=" ";
			rootLogger->debug(str.GetBuffer(0));
		}		
		BYTE *pChr = recBuf.buf + recBuf.currPos; //当前指针;
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


long CBoxSinglechip::fillMedicineFinishBox(MedicineInfo &medInfo)	//盒剂加药结束
{
    //rootLogger->trace("function fillMedicineFinishBox start..............");
	if (runMode == 1 || runMode == 9)
	{
		return 0;
	}
    m_mute->Lock();
	int address = 0; //药品槽位
	long lRet = 0;
	address = medInfo.position;
    rootLogger->debug("加药结束medInfo.position = " + Utils::itos(medInfo.position));
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
			AfxMessageBox("与单片机串口通讯失败！");
			m_mute->Unlock();
			return lRet;
		}          
	}
	int boardNo = 0;  //层号
	int boxId = 0;    //槽号
	int unit = (address-1)/320;//单元
	address -= unit*320;
	boardNo = (address - 1)/BOXES_OF_BOARD; //0-
	boxId = (address - 1)%BOXES_OF_BOARD;   // 0-31  
	
    BYTE szCmd[39];
    int nSize = sizeof(szCmd);
    memset(szCmd, 0, nSize);
    szCmd[0] = 0xEA ;        //贞头
	szCmd[1] = 0xE5;        //贞头
	szCmd[2] = unit;        //单元
	szCmd[3] = boardNo;        //层数  
	szCmd[4] = 0x21;        //长度
	if(unit > 1)
		szCmd[5] = 0x09;        //机械手加药结束命令
	else
	    szCmd[5] = 0x04;        //命令
	
	szCmd[6+boxId] = 1;

	szCmd[nSize - 1] = szCmd[0] + szCmd[1]+ szCmd[2]+ szCmd[3]+ szCmd[4]+ szCmd[5]+ szCmd[6+boxId] ;
	for(int ii = 0; ii < nSize; ii++)
	{
		CString str;
		str.Format("%.2x",szCmd[ii]);
		str+=" ";
		rootLogger->debug(str.GetBuffer(0));
	}
	lRet = port.Write(szCmd, nSize);  //返回是发送成功字符数
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
		while (port.BytesWaiting() == 0 && count >0)				//没有字节直接返回
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
			rootLogger->debug("接收加药结束结果返回总的数据值 count = " + Utils::itos(count));
			int nTmp = 0;
			nTmp = port.Read(recBuf.buf + recBuf.len, count);
			recBuf.len += nTmp;             
			Sleep(100);
		} 
		rootLogger->debug("接收加药结束结果返回的数据:" );
		for( ii = 0; ii < recBuf.len; ii++)
		{
			CString str;
			str.Format("%.2x",recBuf.buf[ii]);
			str+=" ";
			rootLogger->debug(str.GetBuffer(0));
		}
		
		BYTE *pChr = recBuf.buf + recBuf.currPos; //当前指针;
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