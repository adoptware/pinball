#!/usr/bin/make -f
project?=pinball
sudo?=sudo
tarball?=${CURDIR}/../${project}_${version}.orig.tar.gz
export tarball
make?=${MAKE} -f debian/Makefile

version?=$(shell [ -r debian/changelog ] \
  && which dpkg-parsechangelog > /dev/null 2>&1 \
  && dpkg-parsechangelog | grep "Version:" \
  | sed -e "s/Version: \(.*\)-.*/\1/g" \
  || echo "0.0.0")


default: rule/debuild
	-sync

${tarball}:
	 tar cvfz "$@" \
--transform "s|^./|${project}-${version}/|" \
--exclude 'debian' --exclude-vcs \
.
	@ls -l $@

rule/tarball: ${tarball}
	@ls $^

rule/setup: /etc/debian_version
	${sudo} apt-get update -y
	${sudo} apt-get install -y \
build-essential \
dpkg-dev \
devscripts \
debhelper \
\
autoconf \
automake \
libtool-bin \
docbook-utils \
libsdl1.2-dev \
freeglut3-dev \
libsdl-image1.2-dev \
libsdl-mixer1.2-dev \
libogg-dev \
libvorbis-dev \
libaa1-dev \
libtiff-dev \
libcppunit-dev \
# EOL

rule/version:
	@echo ${version}
	-cat /etc/os-release
	${CC} --version
	${MAKE} --version
	aclocal --version
	autoconf --version
	automake --version
	libtool --version
	m4 --version
	pkg-config --version

rule/debuild: debian/rules
	@which ${@F} || ${make} ${@D}/setup
	[ "" != "${tarball}" ] || ${make} ${@D}/tarball
	@ls "${tarball}" 2>/dev/null || ${make} ${@D}/tarball
	${make} ${@D}/version
	${@F} -S -us -uc \
|| echo "# error: Try './debian/rules rule/setup' first"
	${@F} -S -us -uc
	${@F} -us -uc

-include rules
