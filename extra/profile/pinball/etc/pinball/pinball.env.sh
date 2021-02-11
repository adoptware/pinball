#! /bin/sh
# SPDX-License-Identifier: GPL-2.0+

PINBALL_PROFILE="pinball"

# Output screen
# PINBALL_SCREEN="DP-1"

# TODO: Use preferred resolution (align to /etc/default/pinball)
# PINBALL_RESOLUTION="1024x768"

# TODO
PINBALL_EXTRA_DIR="/usr/local/opt/pinball"

PINBALL_IMAGE="/usr/local/opt/pinball/src/pinball/tmp/1024x1024/pinball.jpg"

# Random table if defined
PINBALL_TABLE=

# Quit after displayed frames (for developers)
# PINBALL_QUIT=4294967295
# export PINBALL_QUIT

# Readonly FS
HOME="/tmp/pinball"

# Graphical system : weston or xinit
PINBALL_DISPLAY_MANAGER='weston'
XDG_RUNTIME_DIR='/run/user/0'
#PINBALL_DISPLAY_MANAGER="xinit"
#DISPLAY=':0'
#XINITRC="/etc/pinball/pinball-session.sh"
#PINBALL_XRANDR_ARGS="--rotate left --mode ${PINBALL_RESOLUTION}"

DBUS_SESSION_BUS_ADDRESS='unix:path=/run/user/0/dbus/user_bus_socket'
# ALSA_CARD=""

# Extra
# SDL_VIDEODRIVER=dummy
# SDL_NOMOUSE=
# SDL_AUDIODRIVER=dummy
# SDL_DEBUG=

# XINIT_ARGS= -- -nocursor
