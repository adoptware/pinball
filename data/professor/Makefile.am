AUTOMAKE_OPTIONS = gnu

highscoredir = $(DESTDIR)/$(EM_BUILD_ROOT)/$(EM_HIGHSCORE_DIR)/professor

professorlibdir = $(EM_LIBDIR)
professordatadir = $(datadir)/pinball/professor

professorlib_LTLIBRARIES = libModuleProfessor.la

AM_CPPFLAGS = -I../../base -I.../../addon -I../../src

libModuleProfessor_la_LDFLAGS = -module
libModuleProfessor_la_SOURCES = ModuleProfessor.cpp

professordata_DATA = \
bump.wav \
bumphard.wav \
bumpsoft.wav \
flip.wav \
floor.png \
floor.png.pcx \
gameover.wav \
introp.ogg \
nudge.wav \
pinball.pbl \
professor.ogg \
professor.png

EXTRA_DIST = $(professordata_DATA)
