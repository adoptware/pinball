AUTOMAKE_OPTIONS = gnu

testlibdir = $(EM_LIBDIR)
testdatadir = $(pkgdatadir)

noinst_PROGRAMS = scale simple light texture load explode collision signal billboard font thread menu joy sound trans math misc varray unittest
dnl noinst_PROGRAMS = unittest

INCLUDES = -I../base -I../addon -I../src @INCLTDL@
LDADD = ../src/libemilia_pin.a ../addon/libemilia_addon.a ../base/libemilia_base.a @LIBLTDL@

testlib_LTLIBRARIES = libModuleTest.la

libModuleTest_la_LDFLAGS = -module
libModuleTest_la_SOURCES = moduletest.cpp

unittest_LDFLAGS = -export-dynamic
load_LDFLAGS = -export-dynamic

billboard_SOURCES = billboard.cpp
collision_SOURCES = collision.cpp
explode_SOURCES = explode.cpp
font_SOURCES = font.cpp
joy_SOURCES = joy.cpp
light_SOURCES = light.cpp
load_SOURCES = load.cpp
math_SOURCES = math.cpp
menu_SOURCES = menu.cpp
misc_SOURCES = misc.cpp
scale_SOURCES = scale.cpp
signal_SOURCES = signal.cpp
simple_SOURCES = simple.cpp
sound_SOURCES = sound.cpp
texture_SOURCES = texture.cpp
thread_SOURCES = thread.cpp
trans_SOURCES = trans.cpp
unittest_SOURCES = unittest.cpp
varray_SOURCES = varray.cpp

EXTRA_DIST = \
module.txt \
script.txt \
token.txt
