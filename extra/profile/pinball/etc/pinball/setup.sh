#! /bin/bash
# -*- mode: Bash; tab-width: 2; indent-tabs-mode: nil; coding: utf-8 -*-
# vim:shiftwidth=4:softtabstop=4:tabstop=4:
# SPDX-License-Identifier: GPL-2.0+
set -e
set -x

selfdir=$(dirname -- "$0")
extra_dir=$(realpath -- "${selfdir}/../../../../../extra")
PATH="${selfdir}:${PATH}"
LANG="en_US.UTF-8"
project="pinball"
profile="pincab"
web_url="https://purl.org/rzr/pinball"
git_url="https://github.com/rzr/pinball"
git_branch="master"
# git_branch="sandbox/rzr/devel/master" # TODO
sudo=$(which sudo || echo)
export DEBIAN_FRONTEND=noninteractive

find /etc/pinball ||:
. /etc/os-release ||:
. /etc/pinball/pinball.env.sh \
    || . ${selfdir}/pinball.env.sh ||:

export PINBALL_BRANCH

[ ! -z ${PINBALL_DEVEL} ] || PINBALL_DEVEL=true
[ ! -z ${PINBALL_DIR} ] || PINBALL_DIR="${HOME}/.config/emilia/"

echo "# Update repository"
${sudo} mount -o remount,rw /
[ -e "${HOME}" ] || ${sudo} mkdir -p "${HOME}"
${sudo} sync
${sudo} apt-get update
! ${PINBALL_DEVEL} || ${sudo} apt-get install --yes etckeeper

if true ; then
    echo "# Locale LANG=${LANG}"
    file="/etc/default/locale"
    cat "$file" || ${sudo} apt-get install --yes locales # LANG="C.UTF-8"
    grep "^LANG=${LANG}" "$file" ||\
        echo "LANG=${LANG}" | ${sudo} tee -a "$file"
    grep -v "^#" /etc/locale.gen ||:
    # localepurge will install locale
    ${sudo} DEBIAN_FRONTEND=noninteractive apt-get install --yes localepurge
    echo TODO ${sudo} dpkg-reconfigure locales #
    cat /etc/environment
    time sudo locale-gen --purge "${LANG}" ||:
    du -msc /usr/share/locale/* # ~30M
fi

echo "# Base OS"
echo 'debconf debconf/frontend select Noninteractive' | ${sudo} debconf-set-selections

hostname=$(cat /etc/hostname)
grep $hostname /etc/hosts \
    || { echo "127.0.0.1 $hostname" | $sudo tee -a /etc/hosts ; }

dpkg -L locales || ${sudo} apt-get install -y locales
${sudo} localectl set-keymap en_US.UTF-8 \
    || echo "${LANG} UTF-8" | ${sudo} tee -a /etc/locale.gen
grep -v '^#' /etc/locale.gen

cat<<EOF | ${sudo} tee -a "/etc/default/keyboard"
XKBMODEL="pc105"
XKBLAYOUT="us"
XKBVARIANT="altgr-intl"
XKBOPTIONS=""
BACKSPACE="guess"
EOF

${sudo} apt-get install -y keyboard-configuration
cat "/etc/default/keyboard"


echo "# Main package"
# Suggests: alsa-utils
# Install: xterm x11-utils ...
"${selfdir}/install.sh"

echo "# Needed packages"
${sudo} apt-get install --yes \
          sudo \
          network-manager \
  #EOL

echo "# Hardware support"
if ! true ; then
    case ${HOST_TYPE} in
        *)
            ${sudo} apt-get install --yes \
               linux-image-${HOSTYPE} grub-pc \
               # EOL
            ${sudo} apt-get install --yes \
                grub-invaders memtest86+ \
                # EOL
            ;;
    esac
fi
model=$(head /proc/device-tree/model && echo || echo "generic")

if [[ "$model" =~ "Raspberry Pi" ]] ; then
    pattern='^dtoverlay=vc4'
    list="
/boot/config.txt
/boot/firmware/config.txt
"
    ls /sys/class/drm/card0 || echo "TODO"

    for file in $list ; do
        if [ -f "$file" ] ; then
            grep "$pattern" "$file" \
                || cat<<EOF | sudo tee -a "$file"
#{~${project}: patch $file for $model
dtoverlay=vc4-fkms-v3d, cma-128
#}~${project}
EOF
        fi
    done
fi

echo "# Audio"
which aplay || ${sudo} apt-get install --yes alsa-utils udev

echo "# Graphics system: ${PINBALL_DISPLAY_MANAGER}"
if [ "${PINBALL_DISPLAY_MANAGER}" = "weston" ] ; then
    ${sudo} apt-get install --yes \
          kbd \
          weston \
          # EOL
elif [ "${PINBALL_DISPLAY_MANAGER}" = "xinit" ]; then
    ${sudo} apt-get install --yes \
          xinit \
          x11-xserver-utils \
          xdotool \
          xloadimage \
          # EOL
fi

echo "# Readonly filesystem"
${sudo} mkdir -p "${PINBALL_DIR}"
if [ ! -r /etc/fstab ] \
       || grep '# UNCONFIGURED FSTAB FOR BASE SYSTEM' /etc/fstab ; then \
    cat<<EOF | ${sudo} tee /etc/fstab ||:
# file://etc/fstab # for pinball
none /tmp tmpfs size=16M,mode=1777 0 0
tmpfs /run tmpfs size=32M,nosuid,noexec,relatime,mode=755 0 0
none /var/log tmpfs size=16M,mode=1777 0 0
none ${PINBALL_DIR} tmpfs size=1M,mode=1777 0 0
EOF
fi

echo "# Maintenance mode"
if ${PINBALL_DEVEL} ; then
    ${sudo} apt-get install --yes \
          aptitude \
          avahi-daemon \
          busybox-static \
          iproute2 \
          isc-dhcp-client \
          mosh \
          openssh-server \
          screen \
          time \
          tmux \
          usbutils \
          make \
          zile \
          # EOL

    line='PermitRootLogin yes'
    grep "^$line" /etc/ssh/sshd_config \
        || echo "$line" | ${sudo} tee -a /etc/ssh/sshd_config
fi

if ! true ; then
    echo "# Add Sources for unpackaged file and legal info"
    rm -rf /usr/local/opt/${project}
    mkdir -p /usr/local/opt/${project}/src/${project}
    cd /usr/local/opt/${project}/src/${project}
    git clone --branch "${git_branch}" --depth 1 "${git_url}" .
    echo "# Extra processing from sources"
    ./helper.mk debian/setup
    ./helper.mk tmp/1024x1024/${project}.jpg
    echo "# Add launcher for profile=${profile}"
    cd /usr/local/opt/${project}/src/${project}
    ${sudo} cp -rfav extra/profile/pinball/etc/pinball /etc/
fi
echo "# Add conf files for profile=${profile}"
if [ ! -d '/etc/pinball' ] ; then
    ${sudo} cp -rfav "${extra_dir}/profile/pinball/etc/pinball" "/etc/"
    [ "$profile" = "" ] || \
        ${sudo} cp -rfav "${extra_dir}/profile/${profile}/etc/pinball" "/etc/"
fi

"/etc/pinball/configure.sh"

if true ; then
    cat /etc/issue.net
    for file in /etc/issue /etc/issue.net ; do
        echo -e "${web_url} # for more \n\n$(cat $file)\n" \
            | ${sudo} tee $file.tmp && ${sudo} mv $file.tmp $file
    done
fi

echo "# Cleanup"
${sudo} apt-get install --yes deborphan

if ! ${PINBALL_DEVEL} ; then
    list=" \
      aptitude \
      aptitude-common \
      cron \
      exim4-base \
      exim4-config \
      localepurge \
      mariadb-common \
      mysql-common \
      xz-utils \
      "
    for package in $list ; do
        apt-get remove --yes "${package}" ||:
    done
fi


if ! ${PINBALL_DEVEL} ; then
    echo "# Reducing files"
    list="\
      ${list} \
      ca-certificates \
      gcc-9-base \
      gnupg \
      gnupg-l10n \
      gnupg-utils \
      gpg-wks-client \
      gpg-wks-server \
\
      gpg \
      gpg-agent \
      gpgconf \
      gpgsm \
      gstreamer1.0-plugins-base \
      openssl \
      ppp \
      va-driver-all \
      vdpau-driver-all \
      "
    for package in $list ; do
        ${sudo} dpkg --remove "$package" \
            || dpkg -L "$package" | ${sudo} xargs -n1 rm -v ||:
    done

    df -h
    find /usr/share/doc/ -type f  ! -iname 'copyright*' -exec ${sudo} rm "{}" \;
    ${sudo} rm -rf /usr/share/man/??
    ${sudo} rm -rf /usr/share/man/??_*
    ${sudo} rm -rf /usr/share/bash-completion
    ${sudo} rm -rf /usr/share/fonts
    ${sudo} rm -rf /usr/share/gnupg
    ${sudo} rm -rf /usr/share/info
    ${sudo} rm -rf /usr/share/sounds
fi

${sudo} apt-get autoremove --yes
deborphan --guess-all
${sudo} apt-get remove --yes $(deborphan --guess-all) deborphan ||:

dpkg-query -Wf '${Package} ${Version} ${Installed-Size}\n'

${sudo} apt-get clean
${sudo} rm -rf /var/lib/apt/lists/*

df -m
${sudo} dd if=/dev/zero of=/tmp.tmp ||:
sync
${sudo} rm -f /tmp.tmp
sync

echo "# Reboot"
history -c

cat<<EOF > .bash_history
echo "# ${web_url} # for more"
uptime ; hostname -I ; sudo systemctl stop pinball ; sudo mount -o remount,rw / ; sudo apt-get update ; sudo apt-get upgrade --yes ; df -h ; sudo touch /forcefsck ; sync
sudo reboot
EOF
sync
