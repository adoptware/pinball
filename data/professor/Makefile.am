AUTOMAKE_OPTIONS = gnu

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

EXTRA_DIST = $(professordata_DATA)
