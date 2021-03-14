#! /bin/bash
# -*- mode: Bash; tab-width: 2; indent-tabs-mode: nil; coding: utf-8 -*-
# vim:shiftwidth=4:softtabstop=4:tabstop=4:
# SPDX-License-Identifier: GPL-2.0+

set -x
set -e

self_dir=$(dirname -- "$0")
extra_dir=$(realpath -- "${self_dir}/../../../../../extra")

project="pinball"
sudo=
cd /etc/pinball

. pinball.env.sh

set

${sudo} chmod u+rx ${self_dir}/*.sh

${sudo} install -d /etc/default
${sudo} install ${self_dir}/pinball /etc/default/pinball

${sudo} systemctl get-default | grep 'graphical.target'

${sudo} journalctl --rotate \
    && ${sudo} journalctl --vacuum-time=1s

${sudo} systemctl stop pinball ||:
${sudo} systemctl stop xinit ||:

${sudo} systemctl disable pinball ||:
${sudo} systemctl disable xinit ||:

${sudo} systemctl daemon-reload ||:
# TODO install from /lib/systemd/system/ on debian
${sudo} systemctl enable /etc/${project}/${PINBALL_DISPLAY_MANAGER}.service

if [ "${PINBALL_DISPLAY_MANAGER}" = "weston" ] ; then
    ${sudo} systemctl enable /etc/${project}/${project}.service
    ${sudo} install -d /etc/xdg/weston
    ${sudo} ln -fs ../../${project}/weston.ini /etc/xdg/weston/
fi

cat<<EOF
# Hints for debuging
${sudo} systemctl restart ${PINBALL_DISPLAY_MANAGER}
${sudo} journalctl -u ${PINBALL_DISPLAY_MANAGER} | tail
${sudo} journalctl -u ${PINBALL_DISPLAY_MANAGER} -f | less
$sudo systemd-analyze --user critical-chain
EOF

echo "# Setup terminated, reboot now"
