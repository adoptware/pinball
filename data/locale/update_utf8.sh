#!/bin/bash
echo "UTF-8 is not supported by the Emilia Pinball code."
exit

xgettext -d pinball -o pinball.pot --c++ --from-code=UTF-8 ../../src/*.cpp ../../addon/*.cpp ../../base/*.cpp ../tux/*.cpp ../hurd/*.cpp
msmerge --update --backup=off --no-fuzzy-matching pinball_de_DE_utf8.pot pinball.pot
msgfmt -c -v -o pinball_de_DE_utf8.mo pinball_de_DE_utf8.po
# For this line to have an effect, export LC_ALL=de_DE.UTF-8 and export LANGUAGE=de_DE.utf8 is needed. "utf8" instead of "UTF-8" because of this problem: http://unix.stackexchange.com/questions/186963/what-is-the-proper-encoding-name-to-use-in-locale-for-utf-8
sudo cp pinball_de_DE_utf8.mo /usr/share/locale/de_DE.utf8/LC_MESSAGES/pinball.mo
