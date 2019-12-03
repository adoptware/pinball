# Emilia Pinball README


Emilia Pinball is a open source pinball game for linux.

## Requirements

**Hardware rendering:**
* 300 Mhz cpu.
* OpenGL compatible gfx card.
* SDL 1.2.x, SDL_image and SDL_mixer libraries.

**Recommended:**
* 450 Mhz PIII.
* Decent graphics card, voodoo3, tnt2, geforce, matrox g450 (you do not need anything superfast).
*  Latest SDL libaries, old libraries may have bugs.

## Installing and building

It is best to remove any previous version of pinball before installing a new 
version. If you have used a rpm-package write as root user 'rpm -e pinball'. 
If you have installed from source code goto the pinball source directory and 
write as root user 'make uninstall'.

_NOTE:_ A user and group named 'games' must exist before installation, this
user and group exist by defualt on most systems else:
useradd 'games' (as root user)

After checking that you have the necessary libraries (SDL, SDL_IMAGE and
SDL_MIXER) installed you are ready to install. If you have an rpm-package 
you write as root user 'rpm -i pinball-x.x.x.rpm'.

If you compile from a source package check that you also have the have the 
development libraries installed. They usually have a name like 
sdl-devel.x.x.x.rpm. Then install with the usual:

Did you remember to install the development packages of SDL? Did you remember
to install also SDL_Image and SDL_Mixer?

configure
make 
make install (as root user)
make clean (if you want to save some disc space)

The game is startet with 'pinball'.

## Default Keys

**Esc:** Meny
**Enter:** Launch Ball
**R:** Reset Game
**Left Shift:** Left Flipper
**Left Ctrl:** Nudge from left
**Right Shift:** Right Flipper
**Right Ctrl:** Nudge from right
**Space:** Nudge

Latest news and FAQ is found at http://pinball.sourceforge.net.

Send bug reports (after reading faq) and comments to
henqvist@users.sourceforge.net.

## FAQ

Graphics card known to work: Voodoo3, Matrox G450, GeForce4.
Compiles without problems on: RedHat 7.3, 8.0, 9.0, Debian 3.0.

**Q:** The 'boot' and 'tux' lights on the floor blinks or have vertical
lines?
**A:** Some buggy implementations of OpenGL does not implement the polygon
offset command - which is need by the game for correct rendering.

**Q:** Sounds are delayed with 0.5 seconds?
**A:** You're either using arts, esd or have buggy audio drivers. Stop
arts and esd, if that does not help install alsa drivers.

**Q:** The rpm doesn't fit my dist and the source won't compile?
Search the web for rpms, e.g. Mandrake rpms seems to pop up on
rpmfind a couple of weeks/months after my releases. For compiling:
doublecheck that you the _development_ packages of OpenGL, X, SDL,
SDL_image, SDL_mixer are installed.

**Q:** Can I help?
**A:** Yes, I would like to check that the physics model of the game is
somewhat like a real pinball machine whitout putting all my money into
the pinball games at the local game corner. So if you have a spare
pinball machine send it to me (it don't think it will fit in the
mail though...).
