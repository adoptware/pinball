#!/bin/bash
xgettext -d pinball -o pinball.pot --c++ --from-code=UTF-8 ../../src/*.cpp ../../addon/*.cpp ../../base/*.cpp ../tux/*.cpp ../hurd/*.cpp
msgmerge --update --backup=off --no-fuzzy-matching pinball_de_DE.po pinball.pot
msgfmt -c -v -o pinball_de_DE.mo pinball_de_DE.po
sudo cp pinball_de_DE.mo /usr/share/locale/de_DE/LC_MESSAGES/pinball.mo
