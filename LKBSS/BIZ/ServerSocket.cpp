// ServerSocket.cpp: implementation of the CServerSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerSocket.h"
#include "ConfigUtil.h"
#include "md5.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SendDataLen  1024*16
#define DEFAULT_PORT 2000
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#pragma  comment(lib,"WS2_32.LIB ")
bool CServerSocket::falg=true;
CString CServerSocket::filepath="";
LoggerPtr CServerSocket::rootLogger = Logger::getLogger("CServerSocket");
CServerSocket::CServerSocket()
{
	try{
	
	rootLogger->trace("function CServerSocket start..............");
	int status;
	WSADATA WSAData;
	if (!(status = WSAStartup(MAKEWORD(2,2), &WSAData)) == 0) 
	{
		;
	}
    char path[MAX_PATH];
	memset(path, 0, MAX_PATH);
	GetModuleFileName(NULL, path, MAX_PATH);
	filepath = path; 
	int index = filepath.ReverseFind('\\');   
	filepath = filepath.Left(index); 
	rootLogger->trace("function CServerSocket end..............");
	}catch (...) {
		rootLogger->fatal("CServerSocket--CServerSocket error.");
		return;
	}
}

CServerSocket::~CServerSocket()
{
	try{
	
	rootLogger->trace("function ~CServerSocket start..............");
	falg=false;
	closesocket(m_Listening);
	WSACleanup();
	rootLogger->trace("function ~CServerSocket end..............");
	}catch (...) {
		/// AfxMessageBox("CServerSocket--~CServerSocket error.");
		rootLogger->fatal("CServerSocket--~CServerSocket error.");
		return;
	}
}

int CServerSocket::start()
{
	try{
    sockaddr_in local;
    DWORD dwThreadID = 0;
    local.sin_family = AF_INET;
    local.sin_port = htons(DEFAULT_PORT);
    local.sin_addr.S_un.S_addr = INADDR_ANY;
    m_Listening = socket(AF_INET, SOCK_STREAM, 0);
    if (m_Listening == INVALID_SOCKET)
    {
        return -1;
    }
    if (bind(m_Listening, (LPSOCKADDR) &local, sizeof(local)) == SOCKET_ERROR)
    {
        closesocket(m_Listening);
        return -1 ;
    }
    m_hListenThread = ::CreateThread(NULL, 0, ListenThread, this, 0,&dwThreadID);
    CloseHandle(m_hListenThread);
	return 0;
	}catch (...) {
		/// AfxMessageBox("CServerSocket--start error.");
		rootLogger->fatal("CServerSocket--start error.");
		return -1;
	}
}

DWORD WINAPI CServerSocket::ListenThread(LPVOID lpparam)
{
	try{
	
	CServerSocket *pDlg = (CServerSocket*)lpparam;
    if (pDlg == NULL)
        return -1;
    SOCKET Listening = pDlg->m_Listening;
    if (listen(Listening, 5) == SOCKET_ERROR)
    {
        return -1;
    }
	fd_set fdRecv;
	timeval seltime;
	seltime.tv_sec = 3;
	seltime.tv_usec = 0;
    while (falg)
    {
		FD_ZERO(&fdRecv);
		FD_SET(Listening, &fdRecv);
		
		if(select(0, &fdRecv, NULL, NULL, &seltime) <= 0 || !FD_ISSET(Listening, &fdRecv) )
			continue;
		
		SOCKET ConnectSocket;
        sockaddr_in ClientAddr;
        int nLen = sizeof(sockaddr);
        ConnectSocket = accept(Listening, (sockaddr*) &ClientAddr, &nLen);
		
		FD_ZERO(&fdRecv);
		FD_SET(ConnectSocket, &fdRecv);
		
		if(select(0, &fdRecv, &fdRecv, NULL, &seltime) <= 0  ||!FD_ISSET(ConnectSocket, &fdRecv) )
		{
			closesocket(ConnectSocket);
			continue;
		}
		
		DWORD dwThreadID = 0;
		HANDLE m_hAcceptThread;
		m_hAcceptThread = ::CreateThread(NULL, 0, AcceptThread, &ConnectSocket, 0,&dwThreadID);
        CloseHandle(m_hAcceptThread);
		
    }
	
    return 0;
	}catch (...) {
		/// AfxMessageBox("CServerSocket--ListenThread error.");
		rootLogger->fatal("CServerSocket--ListenThread error.");
		return -1;
	}
}

DWORD WINAPI CServerSocket::AcceptThread(LPVOID lpparam)
{
	try{
	
	SOCKET ConnectSocket;
	ConnectSocket = *(SOCKET*)lpparam;
	CMedicineManage * medmanage1=new CMedicineManage();
	CBIZFillMedicine * fillMed =new CBIZFillMedicine();
	CDBManage *db=new CDBManage();
	while (falg)
    {
		try{	
		char szText[2];
		memset(szText, 0, 2);
		int len=recv(ConnectSocket, szText, 2,0);
		rootLogger->info(szText);
		if (len == 0 || len == SOCKET_ERROR || WSAGetLastError()==WSAECONNRESET)
		{
			closesocket(ConnectSocket);
			break;
		}
		if(strcmp(szText,"1")==0)
		{
			len=send(ConnectSocket, szText, 2,0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
			ClientLocationSet clientlocationset;
			//char *pAddrname = inet_ntoa(ClientAddr.sin_addr);
			len=recv(ConnectSocket, (char *)&clientlocationset, sizeof(clientlocationset), 0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
			//调用处理函数
            if(0==medmanage1->Setclientlocationinfo(clientlocationset))
			{
				strcpy(szText,"0");
				len=send(ConnectSocket, szText, 2,0);
				if (len == SOCKET_ERROR)
				{
					continue;
				}
			}
			else
			{
				len=send(ConnectSocket, szText, 2,0);
				if (len == SOCKET_ERROR)
				{
					continue;
				}
			}			
		}
		else if(strcmp(szText,"2")==0)
		{
			len=send(ConnectSocket, szText, 2,0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
			Prescription1 pre;
			len=recv(ConnectSocket, (char *)&pre, sizeof(pre), 0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
			//调用处理函数
			len=send(ConnectSocket, szText, 2,0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
		}
		else if(strcmp(szText,"3")==0)
		{
			len=send(ConnectSocket, szText, 2,0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
			PrescriptionMed1 preMed;
			len=recv(ConnectSocket, (char *)&preMed, sizeof(preMed), 0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
			//调用处理函数
			len=send(ConnectSocket, szText, 2,0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
		}
		else if(strcmp(szText,"4")==0)
		{
			len=send(ConnectSocket, szText, 2,0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
			PreOperate preOperate;
			len=recv(ConnectSocket, (char *)&preOperate, sizeof(preOperate), 0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
			//调用处理函数
		    if(preOperate.Operatefalg==1)
				;//bizbs->spliceToAbnormalQueueForRemote(preOperate.preId);
			else if(preOperate.Operatefalg==2)
			{
				//list<int>preIdList;
				//preIdList.push_back(preOperate.preId);
				//bizbs->sendToQueueById(&preIdList);
			}
			//Sleep(20);
			len=send(ConnectSocket, szText, 2,0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
		}
		else if(strcmp(szText,"5")==0)//手持终端查询药品信息
		{
			len=send(ConnectSocket, szText, 2,0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
			rootLogger->info("send 5");
			char medscan[50];
			memset(medscan,0,50);
			len=recv(ConnectSocket, medscan, 50, 0);

			rootLogger->info(medscan);
			
			if (len == SOCKET_ERROR)
			{
				continue;
			}
			ClientMedinceLocation clientmedincelocation;
			if(0==medmanage1->QueryLocationMedInfo_Client(medscan,&clientmedincelocation))
			{
	        	len=send(ConnectSocket, szText, 2,0);
				if (len == SOCKET_ERROR)
				{
					continue;
				}
				Sleep(20);
				rootLogger->info("send QueryLocationMedInfo_Client start");
				len=send(ConnectSocket, (char *)&clientmedincelocation, sizeof(clientmedincelocation),0);
				if (len == SOCKET_ERROR)
				{
					continue;
				}
				rootLogger->info("send QueryLocationMedInfo_Client end");
			}
			else
			{
				rootLogger->info(" QueryLocationMedInfo_Client error-----");
				strcpy(szText,"1");
				len=send(ConnectSocket, szText, 2,0);
				if (len == SOCKET_ERROR)
				{
					continue;
				}
			}	
		}
		else if(strcmp(szText,"6")==0)//软件版本更新传送系统文件
		{
			len=send(ConnectSocket, szText, 2,0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
			char buf[SendDataLen];
			memset(buf,0,SendDataLen);
			len=recv(ConnectSocket, buf, SendDataLen, 0);
			CString m_file=filepath+"\\"+buf;
			CFile file;
			CFileStatus fileStatus;	
			ReadCard readcard;
			if(!file.Open(m_file,CFile::modeRead))
			{			
				readcard.basketId=-3;
				send(ConnectSocket,(char *)&readcard, sizeof(readcard),0);	
				/// AfxMessageBox("文件不存在");
				continue;
			}
			file.GetStatus(fileStatus);
			int filelength=fileStatus.m_size;
			BYTE *filebuf=new BYTE[filelength];
			file.ReadHuge(filebuf,filelength);
			file.Close();
			BYTE tk[16];
			MD5_Hash(filebuf,filelength,tk);
			memset(buf,0,1024);
			len=recv(ConnectSocket, buf, 1024, 0);
			if (len == SOCKET_ERROR)
			{
				delete [] filebuf;
				continue;
			}
			BYTE MD5[16];
			memcpy(MD5,buf,len);
			int i;
			for(i=0;i<16;i++)
			{
				if(tk[i]!=MD5[i])
				{
					readcard.basketId=filelength;
					send(ConnectSocket,(char *)&readcard, sizeof(readcard),0);
					FILE *file1;
					file1=fopen(m_file,"rb");
					fseek(file1,0,SEEK_SET);
					int offset=0;
					while(true)
					{
						memset(buf,0,SendDataLen);
						len=fread(buf,1,SendDataLen,file1);
						Sleep(10);
						offset=send(ConnectSocket, buf, len,0);
						if( offset== SOCKET_ERROR && WSAGetLastError()==WSAECONNRESET)
						{
							break;
						}
						if(len<SendDataLen)
						{					
							break;
						}
					}
					fclose(file1); 
					break;
				}
			}
			if(i==16)
			{
				ReadCard readcard;
				readcard.basketId=0;
				send(ConnectSocket,(char *)&readcard, sizeof(readcard),0);		
			}
			delete [] filebuf;
		}
		else if(strcmp(szText,"7")==0)//传送图片
		{
			len=send(ConnectSocket, szText, 2,0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
			rootLogger->info("send 7");
			char buf[SendDataLen];
			memset(buf,0,SendDataLen);
			len=recv(ConnectSocket, buf, SendDataLen, 0);
			CString m_file=filepath+"\\"+buf;
			
			FILE *file1;
			file1=fopen(m_file,"rb");
			ReadCard readcard;
			if(file1 == NULL)
			{			
				readcard.basketId=-3;
				send(ConnectSocket,(char *)&readcard, sizeof(readcard),0);	
				continue;
			}
            fseek(file1,0,SEEK_END);
			int filelength1=ftell(file1);
			fseek(file1,0,SEEK_SET);
			readcard.basketId=filelength1;
			send(ConnectSocket,(char *)&readcard, sizeof(readcard),0);	
			int offset=0;
			while(true)
			{
				memset(buf,0,SendDataLen);
				len=fread(buf,1,SendDataLen,file1);
				Sleep(10);
				send(ConnectSocket, buf, len,0);
				if(len<SendDataLen)
				{
					break;
	
					char buf1[10];
					memset(buf,0,10);
					recv(ConnectSocket, buf1, 10, 0);
					if(strcmp(buf1,"over")==0)
					{
						memset(buf,0,SendDataLen);
				        len=fread(buf,1,SendDataLen,file1);
						if(len==0)
						{
							memset(buf1,0,10);
							strcpy(buf1,"over");
							send(ConnectSocket, buf1, 5,0);
							break;
						}
						else
						{
                            memset(buf1,0,10);
							strcpy(buf1,"start");
							send(ConnectSocket, buf1, 6,0);
							Sleep(10);
							send(ConnectSocket, buf, len,0);
						}
						
					}
				}
			}
			fclose(file1); 
		}
		else if(strcmp(szText,"8")==0)//按地址加药
		{
			len=send(ConnectSocket, szText, 2,0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
			rootLogger->info("send 8");
			AddMedinceInfo addmed;
			len=recv(ConnectSocket, (char *)&addmed, sizeof(addmed), 0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
			//调用处理函数
			if(addmed.oprType==0)
			{
                int res=fillMed->fillMedicineRemote(addmed.medPosition);
				if(res==0)
				{
					rootLogger->info("加药成功");
                    Sleep(50);
					len=send(ConnectSocket, szText, 2,0);
					if (len == SOCKET_ERROR)
					{
						continue;
					}
				}
				else if(res==2)
				{
					rootLogger->info("加药失败 81");// 该地址正在被操作
					Sleep(50);
					szText[0]=81;
					szText[1]='\0';
					len=send(ConnectSocket, szText, 2,0);
					if (len == SOCKET_ERROR)
					{
						continue;
					}
				}
				else if(res==3)
				{
					rootLogger->info("加药失败 83");//该地址已被禁用
					Sleep(50);
					szText[0]=83;
					szText[1]='\0';
					len=send(ConnectSocket, szText, 2,0);
					if (len == SOCKET_ERROR)
					{
						continue;
					}
				}
				else 
				{
					rootLogger->info("加药失败 84");//其他情况
					Sleep(50);
					szText[0]=84;
					szText[1]='\0';
					len=send(ConnectSocket, szText, 2,0);
					if (len == SOCKET_ERROR)
					{
						continue;
					}
				}
			}
			else if(addmed.oprType==1)
			{
				int res=fillMed->finishFillingRemote(addmed.medNum,addmed.medPosition,db);
				if(res==0)
				{
					rootLogger->info("完成加药成功");
                    Sleep(50);
					len=send(ConnectSocket, szText, 2,0);
					if (len == SOCKET_ERROR)
					{
						continue;
					}
				}
				else if( res==2)
				{
					rootLogger->info("完成加药失败 82");//该地址从未开始加药
				   Sleep(50);
					szText[0]=82;
					szText[1]='\0';
					len=send(ConnectSocket, szText, 2,0);
					if (len == SOCKET_ERROR)
					{
						continue;
					}	
				}
				else
				{
					rootLogger->info("完成加药失败 85");//其他情况
					Sleep(50);
					szText[0]=85;
					szText[1]='\0';
					len=send(ConnectSocket, szText, 2,0);
					if (len == SOCKET_ERROR)
					{
						continue;
					}
				}
			}
		}
		else if(strcmp(szText,"9")==0)//手持终端查询位置信息
		{
			len=send(ConnectSocket, szText, 2,0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
			rootLogger->info("send 9");
			int location;
			len=recv(ConnectSocket, (char*)&location, sizeof(location), 0);
			if (len == SOCKET_ERROR)
			{
				continue;
			}
			//调用处理函数
			ClientMedinceInfo clientmedinfo;
			if(0==medmanage1->QueryMedInfo_Client(location, &clientmedinfo))
			{
				len=send(ConnectSocket, szText, 2,0);
				if (len == SOCKET_ERROR)
				{
					continue;
				}
				Sleep(20);
				len=send(ConnectSocket, (char *)&clientmedinfo, sizeof(clientmedinfo),0);
				if (len == SOCKET_ERROR)
				{
					continue;
				}
			}
			else
			{
				strcpy(szText,"1");
				len=send(ConnectSocket, szText, 2,0);
				if (len == SOCKET_ERROR)
				{
					continue;
				}
			}
		}
		Sleep(100);
		}
		catch (...) {
			rootLogger->fatal("CServerSocket--acceptThread error.");
			continue;
		}
	}
	delete fillMed;
	delete medmanage1;
	delete db;
	return 0;
	}catch (...) {
		/// AfxMessageBox("CServerSocket--acceptdata error.");
		rootLogger->fatal("CServerSocket--acceptdata error.");
		return -1;
	}
}


