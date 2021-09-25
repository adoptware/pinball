#!/bin/echo docker build . -f
# -*- coding: utf-8 -*-
# SPDX-License-Identifier: GPL-2.0+

FROM i386/debian:11
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

ENV project pinball
ENV workdir /usr/local/opt/${project}/src/${project}

ADD helper.mk ${workdir}/
WORKDIR ${workdir}
RUN echo "# log: ${project}: Preparing sources" \
  && set -x \
  && ./helper.mk debian/setup/devel \
  && sync

ADD . ${workdir}/
WORKDIR ${workdir}
RUN echo "# log: ${project}: Building sources" \
  && set -x \
  && ./helper.mk debi \
  && dpkg -L ${project} \
  && dpkg -L ${project}-data \
  && sync

RUN echo "# log: ${project}: Listing sources" \
  && set -x \
  && cd .. && find ${PWD}/ -maxdepth 1 \
  && sync


ENTRYPOINT [ "/usr/games/pinball" ]
