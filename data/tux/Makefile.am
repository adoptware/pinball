AUTOMAKE_OPTIONS = gnu

tuxdatadir = $(pkgdatadir)/tux

tuxdata_LTLIBRARIES = libmodule_tux.la

INCLUDES = -I../../base -I.../../addon -I../../visitor -I../../src
dnl libmodule_tux_la_LDADD = ../../src/libemilia_pin.a ../../addon/libemilia_addon.a ../../base/libemilia_base.a
libmodule_tux_la_LDFLAGS = -module
libmodule_tux_la_SOURCES = ModuleTux.cpp

tuxdata_DATA = \
bump.wav \
face.png \
face.png.pcx \
game.mid \
gameover.wav \
flip.wav \
floor2.png \
floor2.png.pcx \
floor.pbl \
intro.mid \
multiball.mid \
nudge.wav \
pinball.pbl \
tiles.png \
tiles.png.pcx \
up.wav

EXTRA_DIST = $(tuxdata_DATA)
