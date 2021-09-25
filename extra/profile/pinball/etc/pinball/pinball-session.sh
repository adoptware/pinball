#! /usr/bin/env bash
# -*- mode: sh; tab-width: 2; indent-tabs-mode: nil; coding: utf-8 -*-
# vim:shiftwidth=4:softtabstop=4:tabstop=4:
# SPDX-License-Identifier: GPL-2.0+
#% * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * %#

set -x
PATH=${PATH}:/usr/games

. /etc/pinball/pinball.env.sh ||:

[ "" != "${PINBALL_BGCOLOR}" ] || PINBALL_BGCOLOR="olive"

set

cat<<EOF
# DISPLAY="${DISPLAY}"
# HOME="${HOME}"
# PWD="${PWD}"
# XDG_RUNTIME_DIR="${XDG_RUNTIME_DIR}"
# WAYLAND_DISPLAY="${WAYLAND_DISPLAY}"
EOF

export PINBALL_TABLE
export HOME
export XDG_RUNTIME_DIR

mkdir -p "${HOME}" ||:

echo "# Delay launcher's gap: ${PINBALL_DISPLAY_MANAGER}"
if [ "weston" = "${PINBALL_DISPLAY_MANAGER}" ] ; then
    while [ ! -e "${XDG_RUNTIME_DIR}/wayland-0" ] ; do
        sleep 1
    done
    sleep 5
fi

echo "# Configure display manager"
ls /sys/class/drm ||:
cat /sys/class/drm/version ||:
ls /sys/class/drm | grep "${PINBALL_SCREEN}" ||:
cat "/sys/class/drm/card0-${PINBALL_SCREEN}/modes" ||:

if [ ! -z ${DISPLAY} ] ; then # X11
    xsetroot -solid "${PINBALL_BGCOLOR}" ||:
    [ ! -r "${PINBALL_XBM_IMAGE}" ] || xsetroot -bitmap "${PINBALL_XBM_IMAGE}"
    [ ! -r "${PINBALL_IMAGE}" ] || xloadimage -onroot "${PINBALL_IMAGE}"

    xrandr
    xrandr --output ${PINBALL_SCREEN} || unset PINBALL_SCREEN

    list=$(xrandr -q \
               | grep '^[^ ].* ' | cut -d' ' -f1 | tail -n +2\
               || echo "")

    # Guess primary screen
    if [ "" = "$PINBALL_SCREEN" ] ; then
        for screen in $list ; do
            xrandr --output "${screen}" --primary ${PINBALL_XRANDR_ARGS} \
                && PINBALL_SCREEN="$screen" && break \
                    || echo "log: skip ${screen}"
        done
    fi

    # Other screen(s)
    for screen in $list ; do
        if [ "${PINBALL_SCREEN}" != "$screen" ] ; then
            xrandr --output "${screen}" --off
            # TODO : It should be above but wont work
            xrandr --output "${screen}" --right-of "${PINBALL_SCREEN}" --auto
        fi
    done

    xrandr --output "${PINBALL_SCREEN}" --primary ${PINBALL_XRANDR_ARGS}
    xdotool mousemove 0 0
fi

echo "# Check audio or guess working output"
aplay /usr/share/games/pinball/tux/lock.wav \
    || for id in $(ls /proc/asound/card*/id \
                       | sed -e 's|/proc/asound/card\(.*\)/id|\1|g' \
                  ) ; do
    export ALSA_CARD="$id"
    aplay /usr/share/games/pinball/tux/bump.wav \
        && break || unset ALSA_CARD
done
echo "# ALSA_CARD=${ALSA_CARD}"
aplay /usr/share/games/pinball/tux/loop.wav ||:


# echo "For maintenance , delay start" && sleep 100

echo "# Launching app"
if [ "xinit" = "${PINBALL_DISPLAY_MANAGER}" ] && [ "" != "$DISPLAY" ] ; then
    # LC_ALL=C twm & # uncomment if needed for debuging
    echo "log: Move to corner"
    {
        window=''
        while [ '' = "$window" ]; do
            window=$(xdotool search -class pinball || echo);
            sleep 1 ;
            xdotool windowmove "$window" 0 0 ;
        done
    } &
fi

# TODO prefix with exec to skip shutdown
pinball

echo "# Quitting app , may shutdown"
# TODO display maintenance images
aplay /usr/share/games/pinball/tux/gameover.wav ||:
sleep 100 # For maintenance stop xinit or pinball service
aplay /usr/share/games/pinball/tux/opentux.wav ||:
reboot # demo mode, comment for poweroff # TODO
poweroff
