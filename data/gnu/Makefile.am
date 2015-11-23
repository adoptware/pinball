AUTOMAKE_OPTIONS = gnu

highscoredir = $(DESTDIR)/$(EM_BUILD_ROOT)/$(EM_HIGHSCORE_DIR)/gnu

gnulibdir = $(EM_LIBDIR)
gnudatadir = $(datadir)/pinball/gnu

gnulib_LTLIBRARIES = libModuleGnu.la

AM_CPPFLAGS = -I../../base -I.../../addon -I../../src

libModuleGnu_la_LDFLAGS = -module
libModuleGnu_la_SOURCES = ModuleGnu.cpp

gnudata_DATA = \
bump.ogg \
bumpsoft.wav \
nudge.wav \
shoot.ogg \
flip2b.ogg \
flip2br.ogg \
lock.ogg \
gnu_letter.ogg \
gnu_all.ogg \
multiball.wav \
gplv1.ogg \
gplv2.ogg \
gplv3.ogg \
freedom0.ogg \
freedom1.ogg \
freedom2.ogg \
freedom3.ogg \
freeball.ogg \
free_letter.ogg \
free_all.ogg \
knockdown.ogg \
reset_knockdowns.ogg \
raise-stopper.ogg \
hit-freeball-target.ogg \
hit-freedom-target.ogg \
plunger.wav \
music.ogg \
pinball.pbl \
gnu.png \
gnu2b.png \
tiles.png \
tiles2.png \
tiles3.png 
#music is Subvulture_-_The_Money_Ran_Out__Rudy, from jamendo.  CC by sa.

# This will create a new empty highscores file
# NOTE! The user games must exist!
install-data-hook:
	-mkdir -p $(highscoredir) # - if not root #!rzr
	-touch $(highscoredir)/highscores
	-if test `whoami` = root; then chown games $(highscoredir)/highscores; fi
	-if test `whoami` = root; then chgrp games $(highscoredir)/highscores; fi
	-chmod 666 $(highscoredir)/highscores

EXTRA_DIST = $(gnudata_DATA)
