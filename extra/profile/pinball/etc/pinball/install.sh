#! /bin/bash
# -*- mode: Bash; tab-width: 2; indent-tabs-mode: nil; coding: utf-8 -*-
# vim:shiftwidth=4:softtabstop=4:tabstop=4:
# SPDX-License-Identifier: GPL-2.0+
set -e
set -x

project='pinball'
[ "" != "$PINBALL_BRANCH" ] || PINBALL_BRANCH='next'
sudo=$(which sudo || echo)

if [ "apt" = "$PINBALL_BRANCH" ] ; then
    ${sudo} apt-get install --yes pinball
else
    ${sudo} apt-get install --yes base-files gnupg curl

    . /etc/os-release
    distro="${ID}_${VERSION_ID}"
    
    if [ "debian" = "${ID}" ] ; then
        [ "${VERSION_ID}" != "" ] || VERSION_ID="10"
        distro="${ID}_${VERSION_ID}"
        [ "${VERSION_ID}" = "10" ] || distro="${distro}.0"
    fi
    if [ "ubuntu" = "$ID" ] ; then
        distro="x${distro}"
    else
        distro=$(echo "${distro}" | sed 's/.*/\u&/')
    fi
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

    package="${project}"
    package_list="${package} \
${project}-table-gnu \
${project}-table-hurd \
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
