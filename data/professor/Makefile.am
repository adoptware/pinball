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

# This will create a new empty highscores file
# NOTE! The user pinball must exist!
install-data-hook:
	touch $(highscoredir)/highscores
	chown pinball $(highscoredir)/highscores
	chgrp pinball $(highscoredir)/highscores
	chmod 666 $(highscoredir)/highscores

EXTRA_DIST = $(professordata_DATA)
