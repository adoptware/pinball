AUTOMAKE_OPTIONS = gnu

highscoredir = $(EM_HIGHSCORE_DIR)/professor

professorlibdir = $(EM_LIBDIR)
professordatadir = $(datadir)/pinball/professor

professorlib_LTLIBRARIES = libModuleProfessor.la

INCLUDES = -I../../base -I.../../addon -I../../src

libModuleProfessor_la_LDFLAGS = -module
libModuleProfessor_la_SOURCES = ModuleProfessor.cpp

professordata_DATA = \
floor.png \
floor.png.pcx \
pinball.pbl \
professor.mid \
professor.png

highscore_DATA = \
highscores

install-data-hook:
#	chown games $(highscoredir)/highscores
#	chgrp users $(highscoredir)/highscores
	chmod 666 $(highscoredir)/highscores

EXTRA_DIST = $(professordata_DATA) $(highscore_DATA)
