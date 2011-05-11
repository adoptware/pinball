#! /usr/bin/make -f
# -*- makefile -*-
# @author: Philippe Coval <http://www.rzr.online.fr>

default: all

PROJECT?=ModulelHurd
src?=$(wildcard *.cpp)
objs?=${src:.cpp=.o}

LDFLAGS=$(shell pinball-config --libs)

LIBS+= \
 /usr/lib/pinball/libemilia_pin.a \
 /usr/lib/pinball/libemilia_base.a \
 /usr/lib/pinball/libemilia_addon.a \

EXTRALIBS+= \
 -lSDL_mixer -lSDL_image -lSDL \
 -lGL -lltdl

CXXFLAGS+= \
 $(shell pkg-config sdl --cflags) \
 $(shell pinball-config --cflags) \
 -DHAVE_CONFIG_H

INSTALL_DIR?=${DESTDIR}/usr/lib/pinball/

all: lib${PROJECT}.so

lib${PROJECT}.so: ${objs}
	${CXX} --shared ${LDFLAGS} -o $@ $^ # ${LIBS}

clean:
	rm *.o *.so

rebuild: clean all


install: lib${PROJECT}.so
	install -d ${INSTALL_DIR}
	install $< ${INSTALL_DIR}/libModuleHurd.so.0.0.0

#eof
