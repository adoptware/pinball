AUTOMAKE_OPTIONS = gnu

bin_PROGRAMS = pinball

noinst_LIBRARIES = libemilia_pin.a
dnl noinst_LTLIBRARIES = libemilia_pin.la

dnl libemilia_pin_la_LDFLAGS = -static

INCLUDES = -I../base -I../addon -I../visitor @INCLTDL@

pinball_LDADD = ../src/libemilia_pin.a ../addon/libemilia_addon.a ../base/libemilia_base.a @LIBLTDL@
pinball_LDFLAGS = -static -export-dynamic

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

noinst_HEADERS = \
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
