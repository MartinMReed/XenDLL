# Microsoft Developer Studio Project File - Name="HGooey" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=HGooey - Win32 SDK Version
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HGooey.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HGooey.mak" CFG="HGooey - Win32 SDK Version"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HGooey - Win32 SDK Version" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "HGooey - Win32 Release_Version" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HGooey - Win32 SDK Version"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "HGooey___Win32_SDK_Version"
# PROP BASE Intermediate_Dir "HGooey___Win32_SDK_Version"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "SDK_Version"
# PROP Intermediate_Dir "SDK_Version"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SDK_Version" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 advapi32.lib gdi32.lib user32.lib d3dx8.lib winmm.lib /nologo /dll /machine:I386 /out:"C:\Program Files\Xenimus\HGooey.dll"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 advapi32.lib gdi32.lib user32.lib d3dx8.lib winmm.lib "HGooey Files/tinyxml/tinyxml.lib" /nologo /dll /machine:I386
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "HGooey - Win32 Release_Version"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "HGooey___Win32_Release_Version"
# PROP BASE Intermediate_Dir "HGooey___Win32_Release_Version"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Version"
# PROP Intermediate_Dir "Release_Version"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Release_Version" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "Release_Version" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 advapi32.lib gdi32.lib user32.lib d3dx8.lib winmm.lib /nologo /dll /machine:I386 /out:"C:\Program Files\Xenimus\HGooey.dll"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 advapi32.lib gdi32.lib user32.lib d3dx8.lib winmm.lib "HGooey Files/tinyxml/tinyxml.lib" /nologo /dll /machine:I386 /out:"C:\Program Files\Xenimus\HGooey.dll"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "HGooey - Win32 SDK Version"
# Name "HGooey - Win32 Release_Version"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=".\HGooey Files\HGooey.cpp"
# End Source File
# Begin Source File

SOURCE=".\HGooey Files\HGuiBase.cpp"
# End Source File
# Begin Source File

SOURCE=".\HGooey Files\HGuiEdit.cpp"
# End Source File
# Begin Source File

SOURCE=".\HGooey Files\HGuiRect.cpp"
# End Source File
# Begin Source File

SOURCE=".\HGooey Files\HGuiWrapper.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "tinyxml"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=".\HGooey Files\tinyxml\tinystr.h"
# End Source File
# Begin Source File

SOURCE=".\HGooey Files\tinyxml\tinyxml.h"
# End Source File
# End Group
# Begin Group "DevIL"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=".\HGooey Files\DevIL\DevIL.h"
# End Source File
# End Group
# Begin Source File

SOURCE=".\HGooey Files\HGIncDef_Private.h"
# End Source File
# Begin Source File

SOURCE=".\HGooey Files\HGIncDef_Public.h"
# End Source File
# Begin Source File

SOURCE=".\HGooey Files\HGooey.h"
# End Source File
# Begin Source File

SOURCE=".\HGooey Files\HGuiBase.h"
# End Source File
# Begin Source File

SOURCE=".\HGooey Files\HGuiRect.h"
# End Source File
# Begin Source File

SOURCE=".\HGooey Files\HGuiWrapper.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
