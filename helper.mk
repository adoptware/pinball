#!/usr/bin/make -f
# -*- makefile -*-
# ex: set tabstop=4 noexpandtab:
# -*- coding: utf-8 -*
#
# SPDX-License-Identifier: GPL-2.0+
# Copyright: Philippe Coval <https://purl.org/rzr/> - 2020+
#

project?=pinball
version?=0.0.0

V?=1

PINBALL_TABLE?=tux
export PINBALL_TABLE

PINBALL_QUIT?=25000
export PINBALL_QUIT

profile?=${project}
export profile
resolution?=1024x1024
export resolution

# profile=pincab # Or overload arg
config_file?=extra/profile/${profile}/etc/${project}/${project}
config_destdir?=${DESTDIR}/etc/${project}

app?=src/${project}

PINBALL_TABLE?=tux
export PINBALL_TABLE

trako_url?=https://github.com/rzr/trako#master
trako_branch?=0.2.0

sudo?=sudo
make?=./helper.mk

tarball?=${CURDIR}/../${project}_${version}.orig.tar.gz
export tarball
topdir?=.

autotools_files+=Makefile
autotools_files+=Makefile.in
autotools_files+=aclocal.m4
autotools_files+=compile
autotools_files+=confdefs.h
autotools_files+=config.status
autotools_files+=configure
autotools_files+=depcomp
autotools_files+=install-sh
autotools_files+=ltmain.sh
autotools_files+=pinconfig.h
autotools_files+=stamp-h1

DESTDIR?=/

debos_suite?=testing
debos_flags?=-v
debos_flags+=--scratchsize=4G
debos_flags+=-tsuite:${debos_suite}
debos_images_lists ?= ${project}-i386.img
debos_images_lists += ${project}-amd64.img


default: help all
	@echo "# $@: @^"

%:
	@ls Makefile || ${make} Makefile
	${MAKE} $@

help: helper.mk
	@echo "## Usage:"
	@echo "# ${<D}/${<F} help # This help"
	@echo "# ${<D}/${<F} run # To run app"
	@echo "# ${<D}/${<F} debi # To install on debian"

config/%: %/config
	@echo "# log: $@: $<"

test/%: %/test
	@echo "# log: $@: $<"


rule/version:
	@echo ${version}
	-cat /etc/os-release
	${CC} --version
	${MAKE} --version
	libtoolize --version
	m4 --version
	aclocal --version
	autoheader --version
	automake --version
	autoconf --version
	pkg-config --version
	-libtool --version

.PHONY: bootstrap
bootstrap: rule/version
	${make} clean/autotools
	${make} configure

helper/%: Makefile
	${MAKE} ${@F}

rule/make: Makefile
	${MAKE}

run: ${app}
	$<

run/quit: ${app}
	SDL_AUDIODRIVER=null \
	$<

start: run

Makefile config.status: configure
	ls $@ || ${<D}/${<F}
	ls $@

acinclude.m4:
	ls $@ || ${make} rule/acinclude.m4
	ls -l $@

rule/acinclude.m4: /usr/share/aclocal/sdl2.m4
	cat $^ | tee -a ${@F}

/usr/share/aclocal/sdl2.m4:
	echo "# TODO: $@ not found"
	echo "# TODO: Please create acinclude.m4 from"
	echo "# https://hg.libsdl.org/SDL/raw-file/d4d66a1891fc/sdl.m4"

configure: acinclude.m4 Makefile.in 
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


debian/setup/devel:
	${sudo} apt-get update
	${sudo} apt-get install -y \
  build-essential \
  dpkg-dev \
  devscripts \
\
  autoconf \
  automake \
  libtool-bin \
  g++ \
  libtool \
  libsdl2-image-dev \
  libsdl2-mixer-dev \
  make \
  pkg-config \
  libogg-dev \
  libvorbis-dev \
  libaa1-dev \
  libtiff-dev \
  libcppunit-dev \
\
  docbook \
  docbook-utils \
  opensp \
  pkg-config \
  sgmlspl \
# EOL

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

debian/setup/package: debian/setup/devel
	${sudo} apt-get update
	${sudo} apt-get install -y \
  debhelper \
  docbook-utils \
  sgmlspl \
# EOL


${tarball}:
	 tar cvfz "$@" \
--transform "s|^./|${project}-${version}/|" \
--exclude 'debian' --exclude-vcs \
.
	@ls -l $@

rule/tarball: ${tarball}
	@ls $^


rule/debuild: debian/rules
	@which ${@F} || ${make} ${@D}/setup
	[ "" != "${tarball}" ] || ${make} ${@D}/tarball
	@ls "${tarball}" 2>/dev/null || ${make} ${@D}/tarball
	${make} ${@D}/version
	${@F} -S -us -uc \
|| echo "# error: Try './debian/rules rule/setup' first"
	${@F} -S -us -uc
	${@F} -us -uc


rule/debi: rule/debuild
	${sudo} ${@F}
	ldd /usr/games/${project}

debi: rule/debi
	@echo "# log: $@: $^"

setup: /etc/os-release
	${make} debian/setup || @echo "TODO: add support for $<"


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


release/%: configure.ac
	git tag -l "${@F}" \
|| echo "# error: Please run: git tag -d ${@F}"
	git tag -l "upstream/${@F}" \
|| echo "error: Please run: git tag -d ${@F}"
	sed -e "s|0.0.0|${@F}|g" -i $<
	-git commit -sam "Release ${@F}"
	git tag -sm "${project}-${@F}" "${@F}"
	git archive \
--format=tar \
--prefix="${project}-${@F}/" \
${@F} \
| gzip -9n \
> ${project}-${@F}.tar.gz
	sha256sum ${project}-${@F}.tar.gz > ${project}-${@F}.tar.gz.sha256sum

docker: docker-compose.yml
	docker-compose up --build

debos/%: % \
 extra/debos \
 extra/profile \
 extra/profile/pinball/etc/pinball/setup.sh
	time debos ${debos_flags} $<


pinball-%.img: extra/debos/machine/%/pinball-%.yaml
	${make} debos/$<

pinball-i386.img: extra/debos/machine/generic/pinball-i386.yaml
	ls $<
	${make} debos/$<

pinball-amd64.img: extra/debos/machine/generic/pinball-amd64.yaml
	${make} debos/$<

%.gz: %
	gzip -f -9 $<
	ls $@

debos/%: pinball-%.img
	ls $^

debos/all: extra/debos/machine/
	for file in $$(find $< -iname "${project}-*.yaml") ; do \
  ${make} debos/$${file} \
  && img=$$(basename $${file} | sed -e 's/.yaml/.img.gz/g') \
  && ${make} $${img} \
  && ls $${img} ;\
done
	@echo "# $@: $^"

debos: debos/i386
	@echo "# $@: $^"

qemu: pinball-i386.img
	kvm -m 512 -device e1000,netdev=net0 -netdev user,id=net0,hostfwd=tcp::55522-:22 -net nic,model=ne2k_pci -net user -machine help -machine pc $<
