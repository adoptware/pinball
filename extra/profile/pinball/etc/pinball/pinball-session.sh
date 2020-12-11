#! /usr/bin/env bash
# -*- mode: sh; tab-width: 2; indent-tabs-mode: nil; coding: utf-8 -*-
# vim:shiftwidth=4:softtabstop=4:tabstop=4:
# XPDX-License-Identifier: GPL-2.0+"
#% * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * %#

set -x
PATH=${PATH}:/usr/games

. /etc/pinball/pinball.env.sh ||:

export PINBALL_TABLE
export HOME
export XDG_RUNTIME_DIR

echo "# Configure display manager"
ls /sys/class/drm ||:

if [ ! -z ${DISPLAY} ] ; then # X11
    xsetroot -solid "purple" ||:
    [ ! -r "${PINBALL_XBM_IMAGE}" ] || xsetroot -bitmap  "${PINBALL_XBM_IMAGE}"
    [ ! -r "${PINBALL_IMAGE}" ] || xloadimage -onroot "${PINBALL_IMAGE}"
    list=$(xrandr -q \
               | grep '^[^ ].* ' | cut -d' ' -f1 | tail -n +2\
               || echo "")
    # Guess primary screen
    if [ "" = "$PINBALL_SCREEN" ] ; then
        for screen in $list ; do
            xrandr --output "${screen}" --primary ${PINBALL_XRANDR_ARGS} \
                && PINBALL_SCREEN="$screen" \
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

# Check audio
aplay /usr/share/games/pinball/tux/lock.wav \
    || cat /proc/asound/card*/id | while read id ; do
        aplay /usr/share/games/pinball/tux/bump.wav \
            && export ALSA_CARD="$id" \
            && break ||:
    done

# echo "For maintenance , delay start" && sleep 100

echo "# Launching app"
aplay /usr/share/games/pinball/tux/loop.wav ||:

if [ "" != "$DISPLAY" ] ; then
    echo "log: Move to corner"
    {
        window=''
        while [ '' = "$window" ]; do
            window=$(xdotool search -class pinball || echo);
            sleep 1 ;
            xdotool windowmove "$window" 0 0 ;
        done
    } &
else
    LC_ALL=C twm & # uncomment if needed for debuging
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
