# Microsoft Developer Studio Project File - Name="Base" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Base - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "base.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "base.mak" CFG="Base - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Base - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Base - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Base - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "c:\tmp\pinball\win32\msvc6\Release\pinball\base"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\\" /I "..\\" /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Base - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir "c:\tmp\pinball\win32\msvc6\Debug\"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\\" /I ".\\" /D "_DEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
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

# Name "Base - Win32 Release"
# Name "Base - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=AlignVisitor.cpp
# End Source File
# Begin Source File

SOURCE=AllegroVisitor.cpp
# End Source File
# Begin Source File

SOURCE=AmbientLightVisitor.cpp
# End Source File
# Begin Source File

SOURCE=Behavior.cpp
# End Source File
# Begin Source File

SOURCE=BehaviorVisitor.cpp
# End Source File
# Begin Source File

SOURCE=BillBoard.cpp
# End Source File
# Begin Source File

SOURCE=Camera.cpp
# End Source File
# Begin Source File

SOURCE=CollisionBounds.cpp
# End Source File
# Begin Source File

SOURCE=CollisionVisitor.cpp
# End Source File
# Begin Source File

SOURCE=Config.cpp
# End Source File
# Begin Source File

SOURCE=EMath.cpp
# End Source File
# Begin Source File

SOURCE=EmFont.cpp
# End Source File
# Begin Source File

SOURCE=Engine.cpp
# End Source File
# Begin Source File

SOURCE=Group.cpp
# End Source File
# Begin Source File

SOURCE=Keyboard.cpp
# End Source File
# Begin Source File

SOURCE=Light.cpp
# End Source File
# Begin Source File

SOURCE=Node.cpp
# End Source File
# Begin Source File

SOURCE=OctTree.cpp
# End Source File
# Begin Source File

SOURCE=OpenGLVisitor.cpp
# End Source File
# Begin Source File

SOURCE=PointLightVisitor.cpp
# End Source File
# Begin Source File

SOURCE=Polygon.cpp
# End Source File
# Begin Source File

SOURCE=Profiler.cpp
# End Source File
# Begin Source File

SOURCE=Shape3D.cpp
# End Source File
# Begin Source File

SOURCE=SignalSender.cpp
# End Source File
# Begin Source File

SOURCE=Sound.cpp
# End Source File
# Begin Source File

SOURCE=SoundUtil.cpp
# End Source File
# Begin Source File

SOURCE=SoundVisitor.cpp
# End Source File
# Begin Source File

SOURCE=TextureUtil.cpp
# End Source File
# Begin Source File

SOURCE=TransformVisitor.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=AlignVisitor.h
# End Source File
# Begin Source File

SOURCE=AllegroVisitor.h
# End Source File
# Begin Source File

SOURCE=AmbientLightVisitor.h
# End Source File
# Begin Source File

SOURCE=BaseTest.h
# End Source File
# Begin Source File

SOURCE=Behavior.h
# End Source File
# Begin Source File

SOURCE=BehaviorVisitor.h
# End Source File
# Begin Source File

SOURCE=BillBoard.h
# End Source File
# Begin Source File

SOURCE=Camera.h
# End Source File
# Begin Source File

SOURCE=CollisionBounds.h
# End Source File
# Begin Source File

SOURCE=CollisionVisitor.h
# End Source File
# Begin Source File

SOURCE=".\config-rzr.h"
# End Source File
# Begin Source File

SOURCE=Config.h
# End Source File
# Begin Source File

SOURCE=EMath.h
# End Source File
# Begin Source File

SOURCE=EmFont.h
# End Source File
# Begin Source File

SOURCE=Engine.h
# End Source File
# Begin Source File

SOURCE=Group.h
# End Source File
# Begin Source File

SOURCE=Keyboard.h
# End Source File
# Begin Source File

SOURCE=Light.h
# End Source File
# Begin Source File

SOURCE=Node.h
# End Source File
# Begin Source File

SOURCE=OctTree.h
# End Source File
# Begin Source File

SOURCE=OpenGLVisitor.h
# End Source File
# Begin Source File

SOURCE=PointLightVisitor.h
# End Source File
# Begin Source File

SOURCE=Polygon.h
# End Source File
# Begin Source File

SOURCE=Private.h
# End Source File
# Begin Source File

SOURCE=Profiler.h
# End Source File
# Begin Source File

SOURCE=Shape3D.h
# End Source File
# Begin Source File

SOURCE=SignalSender.h
# End Source File
# Begin Source File

SOURCE=Sound.h
# End Source File
# Begin Source File

SOURCE=SoundUtil.h
# End Source File
# Begin Source File

SOURCE=SoundVisitor.h
# End Source File
# Begin Source File

SOURCE=StateMachine.h
# End Source File
# Begin Source File

SOURCE=TextureUtil.h
# End Source File
# Begin Source File

SOURCE=TransformVisitor.h
# End Source File
# Begin Source File

SOURCE=Visitor.h
# End Source File
# End Group
# End Target
# End Project
