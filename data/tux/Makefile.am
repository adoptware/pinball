AUTOMAKE_OPTIONS = gnu

highscoredir = $(EM_HIGHSCORE_DIR)/tux

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
face.png \
face.png.pcx \
flip.wav \
floor2.png \
floor2.png.pcx \
game.mid \
gameover.wav \
intro.mid \
lock.wav \
loop.wav \
floor.pbl \
intro.mid \
multiball.mid \
nudge.wav \
opentux.wav \
pinball.pbl \
shoot.wav \
tiles.png \
tiles.png.pcx \
up.wav

highscore_DATA = \
highscores

install-data-hook:
#	chown games $(highscoredir)/highscores
#	chgrp users $(highscoredir)/highscores
	chmod 666 $(highscoredir)/highscores


EXTRA_DIST = $(tuxdata_DATA) $(highscore_DATA)
