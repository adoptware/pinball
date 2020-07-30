#!/usr/bin/make -f
# -*- makefile -*-
# ex: set tabstop=4 noexpandtab:
# -*- coding: utf-8 -*
#
# SPDX-License-Identifier: GPL-2+

project ?= pinball
app ?= src/${project}

trako_url?=https://github.com/rzr/trako#master
trako_branch?=0.2.0

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


trako/%:
	git clone --recursive --depth 1 ${trako_url}  --branch ${trako_branch} trako
	@echo "export CXXFLAGS='-DPINBALL_CONFIG_TRAKO=1'"

trako: trako/src/trako/macros.h trako/README.md
	@grep TRAKO_VERSION "$<"
	@echo "# $@: TODO: Pin version see $<"

trako/remove:
	rm -rf trako
