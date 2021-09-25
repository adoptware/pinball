# EMILIA PINBALL DOCS #

The Emilia Pinball project is an open source pinball simulator
intended for linux machines by Henrik Enqvist.
It has been reported to work on other unix variants also.
Now a days there is also a win***s version, and a mac version might be on its way.

Porting it to other systems should be easy
as long as OpenGL and SDL (or Allegro) are working.

There is both an hardware accelerated version using SDL	and OpenGL
as well as an software version using Allegro available.

The Allegro version is always a bit behind in development
and has more bugs than the SDL/OpenGL version.

## REQUIREMENTS ##

- Some sort of linux or unix box.
- 300 Mhz cpu (450 Mhz for software rendering).
- OpenGL compatible 3D card, OpenGL/Mesa and glut installed.
- The SDL, SDL_image and SDL_mixer library <https://www.libsdl.org>
- Same for <http://alleg.sourceforge.net/>
