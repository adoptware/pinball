#!/bin/echo docker build . -f
# -*- coding: utf-8 -*-
# SPDX-License-Identifier: GPL-2

FROM debian:10
LABEL maintainer="Philippe Coval (rzr@users.sf.net)"

ENV DEBIAN_FRONTEND noninteractive
ENV LC_ALL en_US.UTF-8
ENV LANG ${LC_ALL}

RUN echo "# log: Configuring locales" \
  && set -x \
  && apt-get update -y \
  && apt-get install -y locales \
  && echo "${LC_ALL} UTF-8" | tee /etc/locale.gen \
  && locale-gen ${LC_ALL} \
  && dpkg-reconfigure locales \
  && sync

RUN echo "# log: Setup build system" \
  && set -x \
  && apt-get update -y \
  && apt-get install -y \
  make \
  sudo \
  && apt-get clean \
  && sync

ENV project pinball-table-hurd
ENV workdir /usr/local/opt/${project}/src/${project}

ADD debian/rules ${workdir}/debian/rules
WORKDIR ${workdir}
RUN echo "# log: ${project}: Preparing sources" \
  && set -x \
  && ./debian/rules rule/setup \
  && sync

ADD . ${workdir}/
WORKDIR ${workdir}
RUN echo "# log: ${project}: Building sources" \
  && set -x \
  && ./debian/rules \
  && sudo debi \
  && dpkg -L ${project} \
  && dpkg -L ${project}-data \
  && sync

ENTRYPOINT [ "/usr/games/pinball" ]
