# Microsoft Developer Studio Project File - Name="Speedo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Speedo - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "speedo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "speedo.mak" CFG="Speedo - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Speedo - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Ox /Ot /Og /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "SPEEDO_EXPORTS" /D "_MBCS" /GF PRECOMP_VC7_TOBEREMOVED /c
# ADD CPP /nologo /MT /W3 /GX /Ox /Ot /Og /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "SPEEDO_EXPORTS" /D "_MBCS" /FR /GF /c
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3dx8.lib /nologo /subsystem:windows /dll /machine:IX86 /nodefaultlib:"libci.lib" /def:"Speedo.def" /out:"Release\d3d8.dll" /implib:"$(OutDir)/Speedo.lib" /pdbtype:sept /opt:ref /opt:icf
# ADD LINK32 wpcap.lib wininet.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib shlwapi.lib dinput8.lib d3dx8.lib "../HGooey.DLL/Release_Version/HGooey.lib" "../HGooey.DLL/HGooey Files/tinyxml/tinyxml.lib" /nologo /subsystem:windows /dll /machine:IX86 /nodefaultlib:"LIBC.lib libci.lib" /nodefaultlib:"libc" /def:"Speedo Files/Speedo.def" /out:"C:\Program Files\Xenimus\d3d8.dll" /opt:ref /opt:icf
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "Speedo - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;def;odl;idl;hpj;bat;asm;asmx"
# Begin Source File

SOURCE=".\CustomDevice Files\CustomDevice.cpp"
DEP_CPP_CUSTO=\
	"..\HGooey.DLL\HGooey Files\DevIL\DevIL.h"\
	"..\HGooey.DLL\HGooey Files\HGIncDef_Private.h"\
	"..\HGooey.DLL\HGooey Files\HGIncDef_Public.h"\
	"..\HGooey.DLL\HGooey Files\HGooey.h"\
	"..\HGooey.DLL\HGooey Files\HGuiBase.h"\
	"..\HGooey.DLL\HGooey Files\HGuiRect.h"\
	"..\HGooey.DLL\HGooey Files\HGuiWrapper.h"\
	"..\HGooey.DLL\HGooey Files\tinyxml\tinystr.h"\
	"..\HGooey.DLL\HGooey Files\tinyxml\tinyxml.h"\
	".\CustomDevice Files\CustomDevice.h"\
	".\D3D8 Files\IDirectInput8Hook.h"\
	{$(INCLUDE)}"d3d8.h"\
	{$(INCLUDE)}"d3d8caps.h"\
	{$(INCLUDE)}"d3d8types.h"\
	{$(INCLUDE)}"d3dx8.h"\
	{$(INCLUDE)}"d3dx8core.h"\
	{$(INCLUDE)}"d3dx8effect.h"\
	{$(INCLUDE)}"d3dx8math.h"\
	{$(INCLUDE)}"d3dx8math.inl"\
	{$(INCLUDE)}"d3dx8mesh.h"\
	{$(INCLUDE)}"d3dx8shape.h"\
	{$(INCLUDE)}"d3dx8tex.h"\
	{$(INCLUDE)}"dxfile.h"\
	
# End Source File
# Begin Source File

SOURCE=".\D3D8 Files\IDirect3D8Hook.cpp"
DEP_CPP_IDIRE=\
	"..\HGooey.DLL\HGooey Files\DevIL\DevIL.h"\
	"..\HGooey.DLL\HGooey Files\HGIncDef_Private.h"\
	"..\HGooey.DLL\HGooey Files\HGIncDef_Public.h"\
	"..\HGooey.DLL\HGooey Files\HGooey.h"\
	"..\HGooey.DLL\HGooey Files\HGuiBase.h"\
	"..\HGooey.DLL\HGooey Files\HGuiRect.h"\
	"..\HGooey.DLL\HGooey Files\HGuiWrapper.h"\
	"..\HGooey.DLL\HGooey Files\tinyxml\tinystr.h"\
	"..\HGooey.DLL\HGooey Files\tinyxml\tinyxml.h"\
	".\CustomDevice Files\CustomDevice.h"\
	".\D3D8 Files\IDirect3D8Hook.h"\
	".\D3D8 Files\IDirect3DDevice8Hook.h"\
	".\D3D8 Files\IDirectInput8Hook.h"\
	{$(INCLUDE)}"d3d8.h"\
	{$(INCLUDE)}"d3d8caps.h"\
	{$(INCLUDE)}"d3d8types.h"\
	{$(INCLUDE)}"d3dx8.h"\
	{$(INCLUDE)}"d3dx8core.h"\
	{$(INCLUDE)}"d3dx8effect.h"\
	{$(INCLUDE)}"d3dx8math.h"\
	{$(INCLUDE)}"d3dx8math.inl"\
	{$(INCLUDE)}"d3dx8mesh.h"\
	{$(INCLUDE)}"d3dx8shape.h"\
	{$(INCLUDE)}"d3dx8tex.h"\
	{$(INCLUDE)}"dxfile.h"\
	
# End Source File
# Begin Source File

SOURCE=".\D3D8 Files\IDirect3DDevice8Hook.cpp"
DEP_CPP_IDIREC=\
	"..\HGooey.DLL\HGooey Files\DevIL\DevIL.h"\
	"..\HGooey.DLL\HGooey Files\HGIncDef_Private.h"\
	"..\HGooey.DLL\HGooey Files\HGIncDef_Public.h"\
	"..\HGooey.DLL\HGooey Files\HGooey.h"\
	"..\HGooey.DLL\HGooey Files\HGuiBase.h"\
	"..\HGooey.DLL\HGooey Files\HGuiRect.h"\
	"..\HGooey.DLL\HGooey Files\HGuiWrapper.h"\
	"..\HGooey.DLL\HGooey Files\tinyxml\tinystr.h"\
	"..\HGooey.DLL\HGooey Files\tinyxml\tinyxml.h"\
	".\CustomDevice Files\CustomDevice.h"\
	".\D3D8 Files\IDirect3D8Hook.h"\
	".\D3D8 Files\IDirect3DDevice8Hook.h"\
	".\D3D8 Files\IDirectInput8Hook.h"\
	{$(INCLUDE)}"d3d8.h"\
	{$(INCLUDE)}"d3d8caps.h"\
	{$(INCLUDE)}"d3d8types.h"\
	{$(INCLUDE)}"d3dx8.h"\
	{$(INCLUDE)}"d3dx8core.h"\
	{$(INCLUDE)}"d3dx8effect.h"\
	{$(INCLUDE)}"d3dx8math.h"\
	{$(INCLUDE)}"d3dx8math.inl"\
	{$(INCLUDE)}"d3dx8mesh.h"\
	{$(INCLUDE)}"d3dx8shape.h"\
	{$(INCLUDE)}"d3dx8tex.h"\
	{$(INCLUDE)}"dxfile.h"\
	
# End Source File
# Begin Source File

SOURCE=".\D3D8 Files\IDirectInput8Hook.cpp"
DEP_CPP_IDIRECT=\
	"..\HGooey.DLL\HGooey Files\DevIL\DevIL.h"\
	"..\HGooey.DLL\HGooey Files\HGIncDef_Private.h"\
	"..\HGooey.DLL\HGooey Files\HGIncDef_Public.h"\
	"..\HGooey.DLL\HGooey Files\HGooey.h"\
	"..\HGooey.DLL\HGooey Files\HGuiBase.h"\
	"..\HGooey.DLL\HGooey Files\HGuiRect.h"\
	"..\HGooey.DLL\HGooey Files\HGuiWrapper.h"\
	"..\HGooey.DLL\HGooey Files\tinyxml\tinystr.h"\
	"..\HGooey.DLL\HGooey Files\tinyxml\tinyxml.h"\
	".\CustomDevice Files\CustomDevice.h"\
	".\D3D8 Files\IDirectInput8Hook.h"\
	{$(INCLUDE)}"d3d8.h"\
	{$(INCLUDE)}"d3d8caps.h"\
	{$(INCLUDE)}"d3d8types.h"\
	{$(INCLUDE)}"d3dx8.h"\
	{$(INCLUDE)}"d3dx8core.h"\
	{$(INCLUDE)}"d3dx8effect.h"\
	{$(INCLUDE)}"d3dx8math.h"\
	{$(INCLUDE)}"d3dx8math.inl"\
	{$(INCLUDE)}"d3dx8mesh.h"\
	{$(INCLUDE)}"d3dx8shape.h"\
	{$(INCLUDE)}"d3dx8tex.h"\
	{$(INCLUDE)}"dxfile.h"\
	
# End Source File
# Begin Source File

SOURCE=".\Speedo Files\Speedo.cpp"
DEP_CPP_SPEED=\
	"..\HGooey.DLL\HGooey Files\DevIL\DevIL.h"\
	"..\HGooey.DLL\HGooey Files\HGIncDef_Private.h"\
	"..\HGooey.DLL\HGooey Files\HGIncDef_Public.h"\
	"..\HGooey.DLL\HGooey Files\HGooey.h"\
	"..\HGooey.DLL\HGooey Files\HGuiBase.h"\
	"..\HGooey.DLL\HGooey Files\HGuiRect.h"\
	"..\HGooey.DLL\HGooey Files\HGuiWrapper.h"\
	"..\HGooey.DLL\HGooey Files\tinyxml\tinystr.h"\
	"..\HGooey.DLL\HGooey Files\tinyxml\tinyxml.h"\
	".\CustomDevice Files\CustomDevice.h"\
	".\D3D8 Files\IDirect3D8Hook.h"\
	".\D3D8 Files\IDirect3DDevice8Hook.h"\
	".\D3D8 Files\IDirectInput8Hook.h"\
	{$(INCLUDE)}"d3d8.h"\
	{$(INCLUDE)}"d3d8caps.h"\
	{$(INCLUDE)}"d3d8types.h"\
	{$(INCLUDE)}"d3dx8.h"\
	{$(INCLUDE)}"d3dx8core.h"\
	{$(INCLUDE)}"d3dx8effect.h"\
	{$(INCLUDE)}"d3dx8math.h"\
	{$(INCLUDE)}"d3dx8math.inl"\
	{$(INCLUDE)}"d3dx8mesh.h"\
	{$(INCLUDE)}"d3dx8shape.h"\
	{$(INCLUDE)}"d3dx8tex.h"\
	{$(INCLUDE)}"dxfile.h"\
	
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc;xsd"
# Begin Group "tinyxml"

# PROP Default_Filter "h"
# Begin Source File

SOURCE="..\HGooey.DLL\HGooey Files\tinyxml\tinystr.h"
# End Source File
# Begin Source File

SOURCE="..\HGooey.DLL\HGooey Files\tinyxml\tinyxml.h"
# End Source File
# End Group
# Begin Source File

SOURCE=".\CustomDevice Files\CustomDevice.h"
# End Source File
# Begin Source File

SOURCE="..\HGooey.DLL\HGooey Files\HGIncDef_Private.h"
# End Source File
# Begin Source File

SOURCE="..\HGooey.DLL\HGooey Files\HGIncDef_Public.h"
# End Source File
# Begin Source File

SOURCE="..\HGooey.DLL\HGooey Files\HGooey.h"
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
# Begin Source File

SOURCE=".\D3D8 Files\IDirect3D8Hook.h"
# End Source File
# Begin Source File

SOURCE=".\D3D8 Files\IDirect3DDevice8Hook.h"
# End Source File
# Begin Source File

SOURCE=".\D3D8 Files\IDirectInput8Hook.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx"
# Begin Source File

SOURCE=".\Speedo Files\resource.h"
# End Source File
# Begin Source File

SOURCE=".\Speedo Files\Speedo.def"
# End Source File
# Begin Source File

SOURCE=".\Speedo Files\Speedo.rc"
# End Source File
# End Group
# End Target
# End Project
