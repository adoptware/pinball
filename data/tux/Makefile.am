#$Id: Makefile.am,v 1.12 2003/06/18 10:44:19 henqvist Exp $
AUTOMAKE_OPTIONS = gnu

highscoredir = $(DESTDIR)/$(EM_BUILD_ROOT)/$(EM_HIGHSCORE_DIR)/tux

tuxlibdir = $(EM_LIBDIR)
tuxdatadir = $(datadir)/pinball/tux

tuxlib_LTLIBRARIES = libModuleTux.la

INCLUDES = -I../../base -I.../../addon -I../../src

libModuleTux_la_LDFLAGS = -module
libModuleTux_la_SOURCES = ModuleTux.cpp

tuxdata_DATA = \
bumphard.wav \
bumpsoft.wav \
bump.wav \
face2.png \
face.png.pcx \
flip.wav \
floor2.png \
floor2.png.pcx \
floor3.png \
game.ogg \
gameover.wav \
intro.mid \
lock.wav \
loop.wav \
floor.pbl \
intro.ogg \
multiball.ogg \
nudge.wav \
opentux.wav \
pinball.pbl \
shoot.wav \
tiles.png \
tiles.png.pcx \
up.wav

# This will create a new empty highscores file
# NOTE! The user games must exist!
install-data-hook:
	-mkdir -p $(highscoredir) # - if not root #!rzr
	-touch $(highscoredir)/highscores
	-if test `whoami` = root; then chown games $(highscoredir)/highscores; fi
	-if test `whoami` = root; then chgrp games $(highscoredir)/highscores; fi
	-chmod 666 $(highscoredir)/highscores

EXTRA_DIST = $(tuxdata_DATA)
