#!/usr/bin/make -f
# -*- makefile -*-
# ex: set tabstop=4 noexpandtab:
# -*- coding: utf-8 -*
#
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright: Philippe Coval <https://purl.org/rzr/> - 2020+
#

project?=pinball
V?=1

machine?=generic
architecture?=i386

mirror?=https://deb.debian.org/debian
scratchsize?=4G
suite?=bookworm

flags?=-v
flags+=--scratchsize=${scratchsize}
flags+=-tsuite:${suite}
flags+=-tmirror:${mirror}

images_lists ?= ${project}-i386-generic.img
images_lists += ${project}-amd64-generic.img

topdir?=..
sudo?=sudo


default: help all
	@echo "# $@: @^"

help: Makefile
	@echo "## Usage:"
	@echo "# ${<D}/${<F} help # This help"
	@echo "# ${<D}/${<F} img architecture=${architecture} machine=${machine} # To build image"

all: img

rule/debos/%.yaml: debos/%.yaml \
 profile \
 profile/pinball/etc/pinball/setup.sh
	ls $^
	time debos ${flags} $< | tee ${@F}.log.tmp

rule/debos/all: debos/machine/
	for file in $$(find $< -iname "${project}-*.yaml") ; do \
  ${MAKE} debos/$${file} \
  && img=$$(basename $${file} | sed -e 's/.yaml/.img.gz/g') \
  && ${MAKE} $${img} \
  && ls $${img} ;\
done
	@echo "# $@: $^"

${project}-%-${machine}.img: rule/debos/machine/${machine}/${project}-${machine}.yaml
	du -hsc $@

${project}-%-generic.img: rule/debos/machine/generic/${project}-%.yaml
	du -hsc $@

img: ${project}-${architecture}-${machine}.img
	du -hsc $<

rpi:
	${MAKE} architecture="armel" machine="$@_2" flags="-tkernel:linux-image-rpi"
rpi_2:
	${MAKE} architecture="armhf" machine="$@"
rpi_3_32b:
	${MAKE} architecture="armhf" machine="$@"
rpi_3_64b:
	${MAKE} architecture="arm64" machine="$@"

rpi_3: rpi_3_64b

rpi_4:
	${MAKE} architecture="arm64" machine="$@"

%.gz: %
	stat -c '%y: %n' $^
	gzip -f -9 $<
	stat -c '%y: %n' $@

ram_size?=512
port?=55522
i386/run: pinball-i386-generic.img
	kvm \
	  -m ${ram_size} \
	  -device e1000,netdev=net0 \
	  -netdev user,id=net0,hostfwd=tcp::${port}-:22 \
	  -machine pc $<

bootfile?=/usr/share/OVMF/OVMF_CODE_4M.fd
amd64/run: pinball-amd64-generic.img ${bootfile}
	qemu-system-x86_64 \
	  -enable-kvm \
	  -m ${ram_size} \
	  -cpu host \
	  -drive file=${bootfile},if=pflash,format=raw,unit=0,readonly=on \
	  -drive file=$<,format=raw \
	  -device e1000,netdev=net0 \
	  -netdev user,id=net0,hostfwd=tcp::${port}-:22

run: ${architecture}/run

cleanall:
	rm -rf *.img *.img.gz
