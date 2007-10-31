# Microsoft Developer Studio Project File - Name="HG_SDK" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=HG_SDK - Win32 SDK_Distribution
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HG_SDK.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HG_SDK.mak" CFG="HG_SDK - Win32 SDK_Distribution"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HG_SDK - Win32 SDK_Distribution" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "HG_SDK___Win32_SDK_Distribution"
# PROP BASE Intermediate_Dir "HG_SDK___Win32_SDK_Distribution"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "SDK_Distribution"
# PROP Intermediate_Dir "SDK_Distribution"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SDK_Distribution" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 advapi32.lib gdi32.lib user32.lib "..\HGooey.DLL\SDK_Version\HGooey.lib" /nologo /dll /machine:I386 /out:"C:\Program Files\Xenimus\HG_SDK.dll"
# SUBTRACT BASE LINK32 /incremental:yes
# ADD LINK32 advapi32.lib gdi32.lib user32.lib "../HGooey.DLL/SDK_Version/HGooey.lib" /nologo /dll /machine:I386 /out:"C:\Program Files\Xenimus\HGooey Files\XenDLL\Extensions\HG_SDK\HG_SDK.dll"
# SUBTRACT LINK32 /incremental:yes
# Begin Target

# Name "HG_SDK - Win32 SDK_Distribution"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=".\HG_SDK Files\HG_SDK.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=".\HG_SDK Files\HG_SDK.h"
# End Source File
# Begin Source File

SOURCE="..\HGooey.DLL\HGooey Files\HGIncDef_Public.h"
# End Source File
# Begin Source File

SOURCE="..\HGooey.DLL\HGooey Files\HGuiBase.h"
# End Source File
# Begin Source File

SOURCE="..\HGooey.DLL\HGooey Files\HGuiRect.h"
# End Source File
# Begin Source File

SOURCE="..\HGooey.DLL\HGooey Files\HGuiWrapper.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\HG_SDK Files\HG_SDK.def"
# End Source File
# End Group
# End Target
# End Project
