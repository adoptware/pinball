#! /bin/bash
# -*- mode: Bash; tab-width: 2; indent-tabs-mode: nil; coding: utf-8 -*-
# vim:shiftwidth=4:softtabstop=4:tabstop=4:
# XPDX-License-Identifier: GPL-2.0+"
set -e
set -x

project="pinball"
profile="pincab"
url="https://purl.org/rzr/pinball"
git_url="https://github.com/rzr/pinball"
git_branch="master"
# git_branch="sandbox/rzr/devel/master" # TODO
sudo=$(which sudo || echo)
export DEBIAN_FRONTEND=noninteractive


find /etc/pinball ||:

. /etc/os-release ||:
. /etc/pinball/pinball.env.sh ||:

echo "# Update repository"
mount -o remount,rw /
${sudo} sync
${sudo} apt-get update
${sudo} apt-get install --yes etckeeper

echo "# Base OS"
echo 'debconf debconf/frontend select Noninteractive' | ${sudo} debconf-set-selections

hostname=$(cat /etc/hostname)
grep $hostname /etc/hosts \
    || { echo "127.0.0.1 $hostname" | $sudo tee -a /etc/hosts ; }

dpkg -L locales || ${sudo} apt-get install -y locales
${sudo} localectl set-keymap en_US.UTF-8 \
    || echo 'en_US.UTF-8 UTF-8' | ${sudo} tee -a /etc/locale.gen
cat /etc/locale.gen

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

if [ -z $PINBALL_BRANCH ] ; then
    ${sudo} apt-get install --yes pinball 
else
    ${sudo} apt-get install --yes base-files gnupg curl

    . /etc/os-release
    distro="${ID}_${VERSION_ID}"
    if [ "debian" = "${ID}" ] ; then
        [ "${VERSION_ID}" != "" ] || VERSION_ID="10"
        distro="${ID}_${VERSION_ID}"
        [ "${VERSION_ID}" = "10" ] || distro="${distro}.0"
    elif [ "ubuntu" = "$ID" ] ; then
        distro="x${distro}"
    fi
    distro=$(echo "${distro}" | sed 's/.*/\u&/')
    url="http://download.opensuse.org/repositories"
    url="$url/home:/rzrfreefr"
    [ "${PINBALL_BRANCH}" = "" ] || url="$url:/${PINBALL_BRANCH}"
    url="${url}/${distro}"
    list="/etc/apt/sources.list.d/${project}.list"
    echo "deb [allow-insecure=yes] $url /" | ${sudo} tee "$list"
    suffix="-snapshot"

    curl -s "${url}/Release.key" | gpg --with-fingerprint
    curl "${url}/Release.key" | ${sudo} apt-key add -v -

    ${sudo} apt-get clean
    ${sudo} apt-get update

    package_list="pinball \
pinball-table-gnu \
pinball-table-hurd \
"
    for package in ${package_list} ; do
    
        ${sudo} apt-cache show ${package}
        ${sudo} apt-cache show ${package}${suffix}
        
        version=$(apt-cache show "${package}${suffix}" \
                      | grep 'Version:' | cut -d' ' -f2 | sort -n | head -n1 \
                      || echo 0)
        
        ${sudo} apt-get remove --yes ${package} ||:
        
        ${sudo} apt-get install --yes \
                --allow-downgrades --allow-unauthenticated \
                ${package}${suffix}=${version} \
                ${package}=${version} \
                ${package}-data=${version} \
            ||:
    done
fi


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

echo "# Audio"
which aplay || ${sudo} apt-get install --yes alsa-utils

echo "# Graphics system: ${PINBALL_DISPLAY_MANAGER}"
if [ "${PINBALL_DISPLAY_MANAGER}" = "weston" ] ; then
    ${sudo} apt-get install --yes \
          kbd \
          weston \
          # EOL
    ${sudo} systemctl enable /etc/${project}/${project}.service
    ${sudo} install -d /etc/xdg/weston
    ${sudo} ln -fs ../../${project}/weston.ini /etc/xdg/weston/
elif [ "${PINBALL_DISPLAY_MANAGER}" = "xinit" ]; then
    ${sudo} apt-get install --yes \
          xinit \
          x11-xserver-utils \
          xdotool \
          xloadimage \
          # EOL
fi

echo "# Readonly filesystem"
mkdir -p ${HOME}/.emilia
if [ ! -r /etc/fstab ] \
       || grep '# UNCONFIGURED FSTAB FOR BASE SYSTEM' /etc/fstab ; then \
    cat<<EOF | ${sudo} tee /etc/fstab ||:
# file://etc/fstab # for pinball
none /tmp tmpfs size=16M,mode=1777 0 0
tmpfs /run tmpfs size=32M,nosuid,noexec,relatime,mode=755 0 0
none /var/log tmpfs size=16M,mode=1777 0 0
none /root/.emilia tmpfs size=1M,mode=1777 0 0
EOF
fi

echo "# Maintenance mode"
if ! false; then
    ${sudo} apt-get install --yes \
          aptitude \
          avahi-daemon \
          mosh \
          network-manager \
          openssh-server \
          screen \
          sudo \
          time \
          tmux \
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
    cp -rfav extra/profile/pinball/etc/pinball /etc/
    [ "$profile" = "" ] || cp -rfav extra/profile/${profile}/etc/pinball /etc/
    bash -x -e /etc/pinball/configure.sh
fi

# TODO install from /lib/systemd/system/ on debian
${sudo} systemctl enable /etc/pinball/${PINBALL_DISPLAY_MANAGER}.service


if ! true ; then
    echo "# Theming"
    cat /etc/default/locale || ${sudo} apt-get install locale
    cat /etc/locale.gen ||:
    ${sudo} apt-get install --yes localepurge # will install locale
    ${sudo} dpkg-reconfigure locales # en_US.UTF-8
    cat /etc/default/locale
    cat /etc/environment
    cat /etc/issue.net
    for file in /etc/issue /etc/issue.net ; do
        echo "$url # for more " $(cat $file) \
            | ${sudo} tee $file.tmp && mv $file.tmp $file
    done
fi

echo "# Cleanup"
${sudo} apt-get install --yes deborphan
# remove all locales , en_US.UTF-8
${sudo} apt-get clean
${sudo} rm -rf /var/lib/apt/lists/*
df -h
# /dev/sdb1       3.7G 1020M  2.5G  30% /
deborphan # bsdmainutils

echo "# Reboot"
history -c
echo "# ${url} # for more"
${sudo} apt-get update ; ${sudo} apt-get upgrade --yes
ip addr show ||:
mount -o remount,rw /
echo reboot # TODO
