AUTOMAKE_OPTIONS = gnu

pinlibdir = $(libdir)/pinball
pinincludedir = $(includedir)/pinball

bin_PROGRAMS = pinball

pinlib_LIBRARIES = libemilia_pin.a

INCLUDES = -I../base -I../addon @INCLTDL@

pinball_LDADD = ../src/libemilia_pin.a ../addon/libemilia_addon.a ../base/libemilia_base.a @LIBLTDL@
pinball_LDFLAGS = -export-dynamic

pinball_SOURCES = Pinball.cpp

libemilia_pin_a_SOURCES = \
ArmBehavior.cpp \
BallGroup.cpp \
BounceBehavior.cpp \
BumperBehavior.cpp \
EyeBehavior.cpp \
LoaderModule.cpp \
Loader.cpp \
Obj3dsUtil.cpp \
PlungerBehavior.cpp \
Score.cpp \
Script.cpp \
StateBehavior.cpp \
Table.cpp

dnl libemilia_pin_la_SOURCES = $(libemilia_pin_a_SOURCES)

pininclude_HEADERS = \
ArmBehavior.h \
BallGroup.h \
BounceBehavior.h \
BumperBehavior.h \
EyeBehavior.h \
FakeModuleBehavior.h \
LoaderModule.h \
Loader.h \
Obj3dsUtil.h \
Pinball.h \
PlungerBehavior.h \
Score.h \
Script.h \
SrcTest.h \
StateBehavior.h \
Table.h

# !+rzr : This trick workaround a bad generated rule (under Linux mingw32)
# src/ litool returns:  -L/usr/local//lib /usr/lib/libjpeg.so
# make -C src  wpinball.exe (build the final programm)
# Pinball.o \
# libemilia_pin.a libemilia_addon.a  libemilia_base.a libltdlc.a
wpinball.exe:  ${pinball_OBJECTS} ${pinball_DEPENDENCIES}
	${GCCLOCALDIR}g++ -g -W -Wall -O2 -o$@ $^ \
 -lSDL_mixer -lvorbisfile -lvorbis -logg -lsmpeg \
 -lSDL_image -lSDLmain -lSDL \
 -lglu32 -lopengl32 -ljpeg -lpng  \
 -lwinmm -luser32 -lgdi32 -ldxguid   \
 -lmingw32 -lSDLmain -lSDL  -mwindows \
 -L${GCCLOCALDIR}../lib
# !-rzr : # have to solve it later # -lstdc++
