# Microsoft Developer Studio Project File - Name="addon" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=addon - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "addon.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "addon.mak" CFG="addon - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "addon - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "addon - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "addon - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "\tmp\pinball\win32\msvc6\Release\pinball\addon"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\base\\" /I ".\\" /I "..\\" /I "..\base" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "HAVE_CONFIG_H" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "addon - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir "\tmp\pinball\win32\msvc6\Debug\"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\\" /I "..\base" /I ".\\" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "HAVE_CONFIG_H" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "addon - Win32 Release"
# Name "addon - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=BigSphere.cpp
# End Source File
# Begin Source File

SOURCE=ColorBehavior.cpp
# End Source File
# Begin Source File

SOURCE=Cone.cpp
# End Source File
# Begin Source File

SOURCE=..\base\Config.cpp
# End Source File
# Begin Source File

SOURCE=Cube.cpp
# End Source File
# Begin Source File

SOURCE=Cylinder.cpp
# End Source File
# Begin Source File

SOURCE=Grid.cpp
# End Source File
# Begin Source File

SOURCE=KeyBehavior.cpp
# End Source File
# Begin Source File

SOURCE=KeyRotBehavior.cpp
# End Source File
# Begin Source File

SOURCE=Menu.cpp
# End Source File
# Begin Source File

SOURCE=Sphere.cpp
# End Source File
# Begin Source File

SOURCE=StdAnimation.cpp
# End Source File
# Begin Source File

SOURCE=TexAnimation.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=BigSphere.h
# End Source File
# Begin Source File

SOURCE=ColorBehavior.h
# End Source File
# Begin Source File

SOURCE=Cone.h
# End Source File
# Begin Source File

SOURCE="..\base\config-rzr.h"
# End Source File
# Begin Source File

SOURCE=Cube.h
# End Source File
# Begin Source File

SOURCE=Cylinder.h
# End Source File
# Begin Source File

SOURCE=Grid.h
# End Source File
# Begin Source File

SOURCE=KeyBehavior.h
# End Source File
# Begin Source File

SOURCE=KeyRotBehavior.h
# End Source File
# Begin Source File

SOURCE=Menu.h
# End Source File
# Begin Source File

SOURCE=Sphere.h
# End Source File
# Begin Source File

SOURCE=StdAnimation.h
# End Source File
# Begin Source File

SOURCE=TexAnimation.h
# End Source File
# End Group
# End Target
# End Project
