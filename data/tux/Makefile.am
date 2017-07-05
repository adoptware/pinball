#$Id: Makefile.am $
AUTOMAKE_OPTIONS = gnu

highscoredir = $(DESTDIR)/$(EM_BUILD_ROOT)/$(EM_HIGHSCORE_DIR)/tux

tuxlibdir = $(EM_LIBDIR)
tuxdatadir = $(datadir)/pinball/tux

tuxlib_LTLIBRARIES = libModuleTux.la

AM_CPPFLAGS = -I../../base -I.../../addon -I../../src

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

EXTRA_DIST = $(tuxdata_DATA)
