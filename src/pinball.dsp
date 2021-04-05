# Microsoft Developer Studio Project File - Name="Pinball" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Pinball - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pinball.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pinball.mak" CFG="Pinball - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Pinball - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Pinball - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Pinball - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "c:\tmp\pinball\win32\msvc6\Release\pinball\pinball"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\base\\" /I "..\..\addon\\" /I ".\\" /I "..\\" /I "..\base\\" /I "..\addon\\" /I "..\libltdl\\" /D "NDEBUG" /D "_CONSOLE" /D "WIN32" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib Base.lib addon.lib SDL.lib SDLmain.lib SDL_mixer.lib SDL_image.lib opengl32.lib /nologo /subsystem:windows /machine:I386 /libpath:"..\Release"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Pinball - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir "c:\tmp\pinball\win32\msvc6\Debug\"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\\" /I "..\base\\" /I "..\addon\\" /I "..\libltdl\\" /I ".\\" /D "_DEBUG" /D "_CONSOLE" /D "WIN32" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib Base.lib addon.lib SDL.lib SDLmain.lib SDL_mixer.lib SDL_image.lib opengl32.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\Debug/Pinballd.exe" /pdbtype:sept /libpath:"..\Debug"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "Pinball - Win32 Release"
# Name "Pinball - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=ArmBehavior.cpp
# End Source File
# Begin Source File

SOURCE=.\BallGroup.cpp
# End Source File
# Begin Source File

SOURCE=BounceBehavior.cpp
# End Source File
# Begin Source File

SOURCE=BumperBehavior.cpp
# End Source File
# Begin Source File

SOURCE=EyeBehavior.cpp
# End Source File
# Begin Source File

SOURCE=Loader.cpp
# End Source File
# Begin Source File

SOURCE=.\LoaderModule.cpp
# End Source File
# Begin Source File

SOURCE=.\Obj3dsUtil.cpp
# End Source File
# Begin Source File

SOURCE=Pinball.cpp
# End Source File
# Begin Source File

SOURCE=.\PlungerBehavior.cpp
# End Source File
# Begin Source File

SOURCE=Score.cpp
# End Source File
# Begin Source File

SOURCE=Script.cpp
# End Source File
# Begin Source File

SOURCE=StateBehavior.cpp
# End Source File
# Begin Source File

SOURCE=.\Table.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=ArmBehavior.h
# End Source File
# Begin Source File

SOURCE=BounceBehavior.h
# End Source File
# Begin Source File

SOURCE=BumperBehavior.h
# End Source File
# Begin Source File

SOURCE=EyeBehavior.h
# End Source File
# Begin Source File

SOURCE=FakeModuleBehavior.h
# End Source File
# Begin Source File

SOURCE=Loader.h
# End Source File
# Begin Source File

SOURCE=Pinball.h
# End Source File
# Begin Source File

SOURCE=Score.h
# End Source File
# Begin Source File

SOURCE=Script.h
# End Source File
# Begin Source File

SOURCE=SrcTest.h
# End Source File
# Begin Source File

SOURCE=StateBehavior.h
# End Source File
# Begin Source File

SOURCE=.\Table.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
