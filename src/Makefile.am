AUTOMAKE_OPTIONS = gnu

bin_PROGRAMS = pinball

pkglib_LIBRARIES = libemilia_pin.a

INCLUDES = -I../base -I../addon @INCLTDL@

pinball_LDADD = ../src/libemilia_pin.a ../addon/libemilia_addon.a ../base/libemilia_base.a @LIBLTDL@
pinball_LDFLAGS = -export-dynamic

pinball_SOURCES = Pinball.cpp

libemilia_pin_a_SOURCES = \
ArmBehavior.cpp \
BounceBehavior.cpp \
BumperBehavior.cpp \
EyeBehavior.cpp \
Loader.cpp \
Score.cpp \
Script.cpp \
StateBehavior.cpp

dnl libemilia_pin_la_SOURCES = $(libemilia_pin_a_SOURCES)

pkginclude_HEADERS = \
ArmBehavior.h \
BounceBehavior.h \
BumperBehavior.h \
EyeBehavior.h \
FakeModuleBehavior.h \
Loader.h \
Pinball.h \
Score.h \
Script.h \
SrcTest.h \
StateBehavior.h
