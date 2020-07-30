#!/usr/bin/make -f
# -*- makefile -*-
# ex: set tabstop=4 noexpandtab:
# -*- coding: utf-8 -*
#
# SPDX-License-Identifier: GPL-2+

project ?= pinball
app ?= src/${project}

default: help all
	@echo "# $@: @^"

help: helper.mk
	@echo "## Usage:"
	@echo "# ${<D}/${<F} help # This help"
	@echo "# ${<D}/${<F} run # To run app"


%: Makefile
	${MAKE} $@

rule/make: Makefile
	${MAKE}

run: ${app}
	$<

start: run

Makefile: configure
	${<D}/${<F}

devel: ${app}
	$< || gdb -tui $<

${app}: all
	ls $@

all: rule/make
	${MAKE} all

configure: autogen.sh
	${<D}/${<F}

clean/libs:
	find . -iname "lib*.a" -exec rm -v "{}" \;
