# Microsoft Developer Studio Project File - Name="LKBSS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=LKBSS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LKBSS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LKBSS.mak" CFG="LKBSS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LKBSS - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "LKBSS - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LKBSS - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "BIZ" /I "include" /I "MYSQLinclude" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /pdb:none /machine:I386 /libpath:"lib"

!ELSEIF  "$(CFG)" == "LKBSS - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "BIZ" /I "include" /I "MYSQLinclude" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "LKBSS - Win32 Release"
# Name "LKBSS - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\LKBSS.cpp
# End Source File
# Begin Source File

SOURCE=.\LKBSS.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\LKBSS.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\LKSkinRes\LKDialogRes\BoxPharmacy_off.bmp
# End Source File
# Begin Source File

SOURCE=.\SkinRes\DialogRes\BoxPharmacy_off.bmp
# End Source File
# Begin Source File

SOURCE=.\res\channel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\channel_active.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ColumnHeaderEnd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ColumnHeaderSpan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ColumnHeaderStart.bmp
# End Source File
# Begin Source File

SOURCE=.\res\folder_title_bk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HorizontalScrollBarLeftArrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HorizontalScrollBarRightArrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HorizontalScrollBarSpan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HorizontalScrollBarThumb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LKBSS.ico
# End Source File
# Begin Source File

SOURCE=.\res\LKBSS.rc2
# End Source File
# Begin Source File

SOURCE=.\res\thumb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\thumb_active.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VerticleScrollbarBottom.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VerticleScrollBarDownArrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VerticleScrollBarSpan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VerticleScrollBarThumb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VerticleScrollbarTop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VerticleScrollBarUpArrow.bmp
# End Source File
# End Group
# Begin Group "ControlLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BitmapEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapEdit.h
# End Source File
# Begin Source File

SOURCE=.\BitmapSlider.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapSlider.h
# End Source File
# Begin Source File

SOURCE=.\ImageTabWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageTabWnd.h
# End Source File
# Begin Source File

SOURCE=.\MedBoxCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MedBoxCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MenuBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuBar.h
# End Source File
# Begin Source File

SOURCE=.\RCButton.cpp
# End Source File
# Begin Source File

SOURCE=.\RCButton.h
# End Source File
# Begin Source File

SOURCE=.\RCDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\RCDialog.h
# End Source File
# Begin Source File

SOURCE=.\RCEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\RCEdit.h
# End Source File
# Begin Source File

SOURCE=.\RCList.cpp
# End Source File
# Begin Source File

SOURCE=.\RCList.h
# End Source File
# Begin Source File

SOURCE=.\RCTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\RCTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ScrollHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\ScrollHelper.h
# End Source File
# Begin Source File

SOURCE=.\SkinHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SkinHorizontalScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinHorizontalScrollbar.h
# End Source File
# Begin Source File

SOURCE=.\SkinListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SkinVerticleScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinVerticleScrollbar.h
# End Source File
# Begin Source File

SOURCE=.\XPGroupBox.cpp
# End Source File
# Begin Source File

SOURCE=.\XPGroupBox.h
# End Source File
# End Group
# Begin Group "Util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Keyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\Keyboard.h
# End Source File
# Begin Source File

SOURCE=.\Util.cpp
# End Source File
# Begin Source File

SOURCE=.\Util.h
# End Source File
# End Group
# Begin Group "MainUI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DeviceMonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceMonitor.h
# End Source File
# Begin Source File

SOURCE=.\LKBSSDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LKBSSDlg.h
# End Source File
# Begin Source File

SOURCE=.\WorkQueuePage.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkQueuePage.h
# End Source File
# End Group
# Begin Group "MiscDlg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LogViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\LogViewer.h
# End Source File
# Begin Source File

SOURCE=.\RCMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\RCMessageBox.h
# End Source File
# End Group
# Begin Group "MedicineMgr"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MedicineInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\MedicineInfo.h
# End Source File
# Begin Source File

SOURCE=.\MedicineMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\MedicineMgr.h
# End Source File
# Begin Source File

SOURCE=.\MedLocationEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\MedLocationEditor.h
# End Source File
# End Group
# Begin Group "BIZ"

# PROP Default_Filter ""
# Begin Group "BizGS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BIZ\BIZBaseService.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\BIZBaseService.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\BIZBoxService.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\BIZBoxService.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\BIZFillMedicine.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\BIZFillMedicine.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\BoxMonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\BoxMonitor.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\BoxSinglechip.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\BoxSinglechip.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\DBManage.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\DBManage.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\FxPLC.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\FxPLC.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\MedicineManage.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\MedicineManage.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\PrescriptionManage.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\PrescriptionManage.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\SysConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\SysConfig.h
# End Source File
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BIZ\actfxcpu3.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\actfxcpu3.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\Base64.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\Base64.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\CardManage.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\CardManage.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\ConfigUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\ConfigUtil.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\Des.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\Des.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\Encrypt.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\Encrypt.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\ErrorCodeRes.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\MD5.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\MD5.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\PortManage.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\PortManage.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\SerialPort.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\Serialport.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\ServerSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\ServerSocket.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\Singleton.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\Statistic.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\Statistic.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\tc_32.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\tinystr.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\tinyxml.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\tinyxmlparser.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\ttf2hex.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\ttf2hex.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\utils.cpp
# End Source File
# Begin Source File

SOURCE=.\BIZ\Utils.h
# End Source File
# End Group
# Begin Group "Entities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BIZ\BatchInfo.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\Location.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\Medicine.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\Patient.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\Prescription.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\PrescriptionMedicine.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\WorkflowRecord.h
# End Source File
# Begin Source File

SOURCE=.\BIZ\workQueue.h
# End Source File
# End Group
# End Group
# Begin Group "SystemSetting"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BasicSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\BasicSetting.h
# End Source File
# Begin Source File

SOURCE=.\DatabaseSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DatabaseSetting.h
# End Source File
# Begin Source File

SOURCE=.\PLCSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\PLCSetting.h
# End Source File
# Begin Source File

SOURCE=.\PrinterSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\PrinterSetting.h
# End Source File
# Begin Source File

SOURCE=.\ResetDefaultSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\ResetDefaultSetting.h
# End Source File
# Begin Source File

SOURCE=.\SysMonitorSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\SysMonitorSetting.h
# End Source File
# Begin Source File

SOURCE=.\SystemSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemSetting.h
# End Source File
# End Group
# Begin Group "CAddMedcine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AddMedicineDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AddMedicineDlg.h
# End Source File
# Begin Source File

SOURCE=.\AddMedInfoZonePage.cpp
# End Source File
# Begin Source File

SOURCE=.\AddMedInfoZonePage.h
# End Source File
# Begin Source File

SOURCE=.\OPZonePage.cpp
# End Source File
# Begin Source File

SOURCE=.\OPZonePage.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\LKBSSConfig.xml
# End Source File
# Begin Source File

SOURCE=.\LKBSSErrorInfo.xml
# End Source File
# Begin Source File

SOURCE=.\log4cxx.properties
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section LKBSS : {0051AF37-A877-11D3-83A8-00A024BDBF2B}
# 	2:21:DefaultSinkHeaderFile:actfxcpu3.h
# 	2:16:DefaultSinkClass:CActFXCPU3
# End Section
# Section LKBSS : {CEAA03A5-6C4A-11D5-9312-009099244EFD}
# 	2:5:Class:CActFXCPU3
# 	2:10:HeaderFile:actfxcpu3.h
# 	2:8:ImplFile:actfxcpu3.cpp
# End Section
