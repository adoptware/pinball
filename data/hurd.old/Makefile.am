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
 $(wildcard *.pbl) \
 $(wildcard *.pcx) \
 $(wildcard *.png) \
 $(wildcard *.jpg) \
 $(wildcard *.wav) \
 $(wildcard *.ogg) \
 $(wildcard *.mid) \
 #

# This will create a new empty highscores file
# NOTE! The user games must exist!
install-data-hook:
	-mkdir -p $(highscoredir) # - if not root #!rzr
	-touch $(highscoredir)/highscores
	-if test `whoami` = root; then chown games $(highscoredir)/highscores; fi
	-if test `whoami` = root; then chgrp games $(highscoredir)/highscores; fi
	-chmod 666 $(highscoredir)/highscores

EXTRA_DIST = $(gnudata_DATA)
