#!/usr/bin/make -f
# -*- makefile -*-
# ex: set tabstop=4 noexpandtab:
# -*- coding: utf-8 -*
#
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright: Philippe Coval <https://purl.org/rzr/> - 2021+
#

default: help

sudo?=sudo
srcs?=$(wildcard *.md | sort)
home?=Home.md
branch?=$(shell git rev-parse --abbrev-ref HEAD)
url?=http://localhost:4567

help:
	@echo "Usage:"
	@echo "make setup start"

setup/debian: /etc/debian_version
	${sudo} apt-get -y update
	${sudo} apt-get -y install libicu-dev cmake ruby-dev
	${sudo} gem install github-linguist
	${sudo} gem install gollum
	${sudo} gem install org-ruby  # optional

setup: setup/debian
	-@sync

run: ${home}
	cd ../docs/.. \
	&& gollum --ref ${branch} --page-file-dir docs 

${home}: ${srcs}
	echo $^ | tr ' ' '\n' |  while read  file; do echo "- [$$file]($$file)" ; done > $@
	-git add "$@"
	-git commit -sm 'docs: Regenerated default file' "$@"

home: ${home}
	cat $<

regen:
	rm -f ${home}
	make home
start:
	x-www-browser "${url}"
	${MAKE} run
	echo "info: Reload ${url}"
