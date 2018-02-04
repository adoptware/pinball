#!/bin/bash
xgettext -d pinball -o pinball.pot --c++ --from-code=UTF-8 ../../src/*.cpp ../../addon/*.cpp ../../base/*.cpp ../tux/*.cpp ../hurd/*.cpp
msgmerge --update --backup=off --no-fuzzy-matching de_DE.po pinball.pot
msgfmt -c -v -o de_DE.mo de_DE.po
msgmerge --update --backup=off --no-fuzzy-matching fr_FR.po pinball.pot
msgfmt -c -v -o fr_FR.mo fr_FR.po
msgmerge --update --backup=off --no-fuzzy-matching pt_PT.po pinball.pot
msgfmt -c -v -o pt_PT.mo pt_PT.po
#sudo cp pinball_de_DE.mo /usr/share/locale/de_DE/LC_MESSAGES/pinball.mo
