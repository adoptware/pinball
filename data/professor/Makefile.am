AUTOMAKE_OPTIONS = gnu

professordatadir = $(pkgdatadir)/professor

professordata_LTLIBRARIES = libmodule_professor.la

INCLUDES = -I../../base -I.../../addon -I../../visitor -I../../src
dnl libmodule_tux_la_LDADD = ../../src/libemilia_pin.a ../../addon/libemilia_addon.a ../../base/libemilia_base.a
libmodule_professor_la_LDFLAGS = -module
libmodule_professor_la_SOURCES = ModuleProfessor.cpp

professordata_DATA = \
floor.png \
floor.png.pcx \
pinball.pbl \
professor.mid

EXTRA_DIST = $(professordata_DATA)
