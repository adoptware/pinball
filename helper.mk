#!/usr/bin/make -f
# -*- makefile -*-
# ex: set tabstop=4 noexpandtab:
# -*- coding: utf-8 -*
#
# SPDX-License-Identifier: GPL-2+

project ?= pinball

PINBALL_TABLE ?= tux
export PINBALL_TABLE

PINBALL_QUIT=25000
export PINBALL_QUIT

app ?= src/${project}

trako_url?=https://github.com/rzr/trako#master
trako_branch?=0.2.0

sudo ?= sudo
make ?= ./helper.mk

autotools_files += configure
autotools_files += Makefile
autotools_files += Makefile.in
autotools_files += aclocal.m4
autotools_files += depcomp
autotools_files += compile
autotools_files += pinconfig.h
autotools_files += stamp-h1
autotools_files += config.status
autotools_files += install-sh
autotools_files += ltmain.sh
autotools_files += confdefs.h

default: help all
	@echo "# $@: @^"

help: helper.mk
	@echo "## Usage:"
	@echo "# ${<D}/${<F} help # This help"
	@echo "# ${<D}/${<F} run # To run app"

version:
	${MAKE} --version
	libtoolize --version
	aclocal --version
	autoheader --version 
	automake --version
	autoconf --version
	pkg-config --version

.PHONY: bootstrap
bootstrap: version
	${make} clean/autotools configure

helper/%: Makefile
	${MAKE} ${@F}

rule/make: Makefile
	${MAKE}

run: ${app}
	$<

start: run

Makefile config.status: configure
	ls $@ || ${<D}/${<F}
	ls $@

acinclude.m4:
	ls $@ && exit 0
	${make} rule/acinclude.m4

rule/acinclude.m4: /usr/share/aclocal/sdl.m4
	cat $< >> ${@F}

/usr/share/aclocal/sdl.m4:
	echo "# TODO: $@ not found"
	echo "# TODO: Please create acinclude.m4 from"
	echo "# https://hg.libsdl.org/SDL/raw-file/d4d66a1891fc/sdl.m4"

configure: Makefile.in
	autoconf
	ls $@

depcomp install-sh: ltmain.sh pinconfig.h.in README
	@echo "# log: $@: $<"
	automake --add-missing
	ls $@

Makefile.in: Makefile.am install-sh
	ls $@

README: README.md
	ln -fs $< $@
	ls $@

AUTHORS ChangeLog NEWS:
	touch $@

rule/autoupdate: configure.ac
	${@F}

aclocal.m4:
	aclocal
	ls $@

pinconfig.h.in: aclocal.m4
	autoheader
	ls $@

ltmain.sh:
	libtoolize --ltdl --force --copy
	ls $@

devel: ${app}
	$< || gdb -tui $<

${app}: all
	ls $@

all: rule/make
	${MAKE} all

clean/autotools:
	rm -rf ${autotools_files}
clean/libs:
	find . -iname "lib*.a" -exec rm -v "{}" \;

install/config: extra/config/${project}
	${sudo} install -d ${DESTDIR}/etc/default/
	${sudo} install $< ${DESTDIR}/etc/default/

test/config: install/config
	-mv -f ${HOME}/.emilia/pinball ${HOME}/.emilia/pinball.bak
	${make} run

trako/%:
	git clone --recursive --depth 1 ${trako_url}  --branch ${trako_branch} trako
	@echo "export CXXFLAGS='-DPINBALL_CONFIG_TRAKO=1'"

trako: trako/src/trako/macros.h trako/README.md
	@grep TRAKO_VERSION "$<"
	@echo "# $@: TODO: Pin version see $<"

trako/remove:
	rm -rf trako
