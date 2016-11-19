#$Id: Makefile.am $
AUTOMAKE_OPTIONS = gnu

highscoredir = $(DESTDIR)/$(EM_BUILD_ROOT)/$(EM_HIGHSCORE_DIR)/hurd

hurdlibdir = $(EM_LIBDIR)
hurddatadir = $(datadir)/pinball/hurd

hurdlib_LTLIBRARIES = libModuleHurd.la

INCLUDES = -I../../base -I.../../addon -I../../src

libModuleHurd_la_LDFLAGS = -module
libModuleHurd_la_SOURCES = ModuleHurd.cpp

hurddata_DATA = \
 $(wildcard *.wav) \
 $(wildcard *.png) \
 $(wildcard *.pcx) \
 $(wildcard *.ogg) \
 $(wildcard *.mid) \
 $(wildcard *.pbl) \
 $(wildcard *.jpg) \
 #

EXTRA_DIST = $(gnudata_DATA)
