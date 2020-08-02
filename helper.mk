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

profile ?= ${project}
export profile
resolution ?= 1024x1024
export resolution

# profile=pincab # Or overload arg
config_file ?= extra/profile/${profile}/etc/${project}/${project}
config_destdir ?= ${DESTDIR}/etc/${project}

app ?= src/${project}

PINBALL_TABLE ?= tux
export PINBALL_TABLE

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

config/%: %/config
	@echo "# log: $@: $<"

test/%: %/test
	@echo "# log: $@: $<"


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

config/bak:
	-mv -f ${HOME}/.emilia/pinball ${HOME}/.emilia/pinball.bak

config/etc/install: ${config_destdir}/${project}
	ls $<

config/etc/delete:
	${sudo} rm -fv ${config_destdir}/${project}

${config_destdir}/${project}: ${config_file}
	${sudo} install -d ${@D}
	${sudo} install $< ${@}

config/install: ${config_file} config/bak
	install -d ${HOME}/.emilia
	install $< ${HOME}/.emilia/

config/test: config/etc/delete config/etc/install config/bak run
	@echo "# log: $@: $<"

config/run: config/install run
	@echo "# log: $@: $<"


pincab/run:
	${make} config/run profile="${@D}"

debian/setup/x11: /etc/debian_version
	${sudo} apt-get update
	${sudo} apt-get install -y \
  xinit \
  x11-xserver-utils
# EOL

debian/setup: /etc/debian_version
	${sudo} apt-get update
	${sudo} apt-get install -y \
  graphicsmagick-imagemagick-compat \
  make \
  sudo \
# EOL

%.gz: %
	gzip -9 $<


${project}.xpm/%: data/splash.png
	install -d tmp/${@F}
	convert -resize ${@F} -depth 8 -colors 14 $< tmp/${@F}/${@D}

${project}.tga/%: data/splash.png
	install -d tmp/${@F}
	convert -resize ${@F} -depth 8 -colors 14 + dither $< tmp/${@F}/${@D}

${project}.png/%: data/splash.png
	install -d tmp/${@F}
	convert -resize ${@F} $< tmp/${@F}/${@D}

${project}.jpg/%: data/splash.png
	install -d tmp/${@F}
	convert -resize ${@F} $< tmp/${@F}/${@D}

640x480/%:
	${make} resolution=${@D} ${F}

tmp/${resolution}/${project}.jpg:
	ls $@ || ${make} ${@F}/${resolution}

jpg: tmp/${resolution}/${project}.jpg
	file $<

pincab:
	${make} resolution=1024x1024 jpg

# https://wiki.debian.org/Grub/SplashImage
grub: extra/${project}.xpm.gz  /etc/default/grub
	echo "sudo install $<.gz /grub/splashimages/"
	echo "splashimage=(hd0,0)/grub/$<"
	echo "GRUB_CMDLINE_LINUX_DEFAULT=\"quiet splash\""

/grub/splashimages/${project}.%: tmp/640x480/${project}.png
	${sudo} install -d ${@D}
	${sudo} install $< $@

grub2: /grub/splashimages/${project}.xpm.gz
	ls $<

trako/%:
	git clone --recursive --depth 1 ${trako_url}  --branch ${trako_branch} trako
	@echo "export CXXFLAGS='-DPINBALL_CONFIG_TRAKO=1'"

trako: trako/src/trako/macros.h trako/README.md
	@grep TRAKO_VERSION "$<"
	@echo "# $@: TODO: Pin version see $<"

trako/remove:
	rm -rf trako
